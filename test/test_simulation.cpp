#include <stdio.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <tr1/random>
#include <UnitTest++.h>
#include <ecalc/ecalc.hpp>
#include <showdown_eval.hpp>
#include <weighted_showdown_eval.hpp>
#include <fcontext.hpp>
#include <fplayer.hpp>
#include <fseat.hpp>
#include <fcontext_config.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/array_handlist.hpp>
#include <ecalc/random_handlist.hpp>
#include <mcts/running_stats.hpp>
#include <bucket_collection.hpp>
#include <weighted_bucket_handlist.hpp>

SUITE(SimulationTests) {

  using namespace poker;
  using namespace freedom;

  using ecalc::Handlist;
  using ecalc::Handranks;
  using ecalc::RandomHandlist;
  using ecalc::SingleHandlist;

  ecalc::Handranks handranks("../../../bin/data/handranks.dat");

  struct TestStartup {
      ecalc::ECalc *calc;
    std::mt19937 nb_gen;

    FContextConfig *cconfig;
    vector<FSeat> players;
    FContext *context;
    vector<Action> actions, expected;
    bb pot;
    bb highest_bet;
    int index_bot;
    int index_utg;
    int index_button;
    int index_active;
    int betting_round;
    PhaseType::Enum phase;
    vector<unsigned> board;

    Handlist *bot_hl;
    Handlist *random;

    TestStartup()
        : players({FSeat(FPlayer("mark", bb(100),
                                 vector<bb>({bb(0), bb(0),
                                                 bb(0), bb(0)})),
                         StatusType::Active),
                   FSeat(FPlayer("simon", bb(9.5),
                                 vector<bb>({bb(0.5), bb(0),
                                                 bb(0), bb(0)})),
                         StatusType::Active),
                   FSeat(FPlayer("fish", bb(9),
                                 vector<bb>({bb(1), bb(0),
                                                 bb(0), bb(0)})),
                         StatusType::Active)}),
          random(new RandomHandlist()) {
      calc = new ECalc(&handranks, 0);
      // seeded with zero, so results are independent of randomness
      nb_gen = std::mt19937(0);

      pot = bb(1.5);
      highest_bet = bb(1);
      index_bot = 0;
      index_utg = 0;
      index_button = 0;
      index_active = 0;
      betting_round = 0;
      board = vector<unsigned>();
      phase = PhaseType::Preflop;
      bot_hl = new SingleHandlist(Hand("AhAs"));

      players[0].player.handlist = random;
      players[1].player.handlist = random;
      players[2].player.handlist = random;

      cconfig = new FContextConfig(Hand("AhAs"), 2, board, vector<double>({1}),
                                   vector<double>({3}));
      context =
          new FContext(pot, highest_bet, index_bot, index_utg, index_button,
                       index_active, betting_round, phase, players,
                       Action(ActionType::None, bb(0)), cconfig);
    }

    ~TestStartup() {
        delete calc;

      delete context;
      delete cconfig;
      delete random;
      delete bot_hl;
    }
  };

  TEST_FIXTURE(TestStartup, TestInit) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 1000);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestFixedWin) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 1000);

    context->seats[0].player.make_investment(bb(100), PhaseType::Preflop);
    context->seats[1].player.make_investment(bb(9.5), PhaseType::Preflop);
    context->seats[2].player.make_investment(bb(9), PhaseType::Preflop);

    bb fixed_win = evaluator->get_fixed_win(*context);
    CHECK_EQUAL(bb(90), fixed_win);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestFixedWin2) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 1000);

    context->seats[0].player.make_investment(bb(5), PhaseType::Preflop);
    context->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);
    context->seats[2].player.make_investment(bb(9), PhaseType::Preflop);

    bb fixed_win = evaluator->get_fixed_win(*context);
    CHECK_EQUAL(bb(0), fixed_win);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestNotWinnablePot) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 1000);

    context->seats[0].player.bankroll = 3;
    context->seats[0].player.make_investment(bb(3), PhaseType::Preflop);
    context->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);
    context->seats[2].player.make_investment(bb(4), PhaseType::Preflop);

    bb fixed_no_win = evaluator->get_not_winnable_pot(*context);
    CHECK_EQUAL(bb(4), fixed_no_win);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestNotWinnablePot2) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 1000);

    context->seats[0].player.bankroll = bb(100);
    context->seats[0].player.make_investment(bb(100), PhaseType::Preflop);
    context->seats[1].player.make_investment(bb(9.5), PhaseType::Preflop);
    context->seats[2].player.make_investment(bb(9), PhaseType::Preflop);

    bb fixed_no_win = evaluator->get_not_winnable_pot(*context);
    CHECK_EQUAL(bb(0), fixed_no_win);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContext) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 17);

    context->pot = bb(120);
    context->seats[0].player.bankroll = bb(100);
    context->seats[0].player.make_investment(bb(100), PhaseType::Preflop);
    context->seats[1].player.make_investment(bb(9.5), PhaseType::Preflop);
    context->seats[2].player.make_investment(bb(9), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 100; ++i)
      stats.push(evaluator->simulate(*context));

    /**
     * the mean should converge to zero.
     * important note:
     * the number of times the value
     * from simulate(context) is calculated is critical. if the sampling rate is
     * low ( 7 - 100 samples ) the deviation from the real value is quite
     * high. so the node has to be viewed more often to get an accurate result.
     * respectively if the number of samples per run is high ( > 500 ? )
     * the real value is approximated much quicker.
     * TODO benchmark these values
     **/

    // std::cout << stats.mean() << std::endl;
    // std::cout << stats.standard_deviation() << std::endl;
    CHECK_CLOSE(0, stats.mean(), 0.1);

    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContextAhAs) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 7);

    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                bot_hl),
        StatusType::Active);

    vector<FSeat> seats({mark, players[1]});
    FContextConfig *ccon = new FContextConfig(
        Hand("AhAs"), 2, board, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    con->pot = 10;
    // context->seats[0].player.bankroll = 100;
    con->seats[0].player.make_investment(bb(5), PhaseType::Preflop);
    con->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 1000; ++i)
      stats.push(evaluator->simulate(*con));

    CHECK_CLOSE(3.5, stats.mean(), 0.1);

    delete ccon;
    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContextAhAsWithRake) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 7);

    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                bot_hl),
        StatusType::Active);

    vector<FSeat> seats({mark, players[1]});
    FContextConfig *ccon = new FContextConfig(
        Hand("AhAs"), 2, board, vector<double>({1}), vector<double>({3}),
        0.5); // rake of 0.5 for this test, normally much lower
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    con->pot = bb(10);
    // context->seats[0].player.bankroll = 100;
    con->seats[0].player.make_investment(bb(5), PhaseType::Preflop);
    con->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 1000; ++i)
      stats.push(evaluator->simulate(*con));

    CHECK_CLOSE(1.75, stats.mean(), 0.1);

    delete ccon;
    delete evaluator;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContext7h2c) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 7);
    Handlist *single = new SingleHandlist(Hand("7h2c"));
    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                bot_hl),
        StatusType::Active);

    mark.player.handlist = single;
    vector<FSeat> seats({mark, players[1]});
    FContextConfig *ccon = new FContextConfig(
        Hand("AhAs"), 2, board, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    con->pot = bb(10);
    // context->seats[0].player.bankroll = 100;
    con->seats[0].player.make_investment(bb(5), PhaseType::Preflop);
    con->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 1500; ++i)
      stats.push(evaluator->simulate(*con));

    CHECK_CLOSE(-1.6, stats.mean(), 0.1);

    delete ccon;
    delete con;
    delete evaluator;
    delete single;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContextNotWinnable) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 7);
    Handlist *single = new SingleHandlist(Hand("AhAs"));
    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                bot_hl),
        StatusType::Active);

    vector<FSeat> seats({mark, players[1]});
    FContextConfig *ccon = new FContextConfig(
        Hand("AhAs"), 2, board, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    con->pot = bb(110);
    con->seats[0].player.bankroll = bb(10);
    con->seats[1].player.bankroll = bb(99.5);
    con->seats[0].player.make_investment(bb(10), PhaseType::Preflop);
    con->seats[1].player.make_investment(bb(99.5), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 1500; ++i)
      stats.push(evaluator->simulate(*con));

    CHECK_CLOSE(7, stats.mean(), 0.1);

    delete ccon;
    delete con;
    delete evaluator;
    delete single;
  }

  TEST_FIXTURE(TestStartup, TestSimulateContextThatOneIsWon) {
    ShowdownEval *evaluator = new ShowdownEval(calc, 7);
    Handlist *single = new SingleHandlist(Hand("AhAs"));
    vector<unsigned> bboard(
        {Card("Ac").card(), Card("Ad").card(), Card("7c").card()});
    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                bot_hl),
        StatusType::Active);

    vector<FSeat> seats({mark, players[1]});
    FContextConfig *ccon = new FContextConfig(
        Hand("AhAs"), 2, bboard, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    con->pot = 10;
    con->seats[0].player.bankroll = 10;
    // con->seats[1].player.bankroll = 99.5;
    con->seats[0].player.make_investment(bb(5), PhaseType::Preflop);
    con->seats[1].player.make_investment(bb(4.5), PhaseType::Preflop);

    mcts::RunningStats stats;
    for (int i = 0; i < 1000; ++i)
      stats.push(evaluator->simulate(*con));

    CHECK_CLOSE(4.99, stats.mean(), 0.1);

    delete ccon;
    delete con;
    delete evaluator;
    delete single;
  }

  TEST_FIXTURE(TestStartup, TestApplyExpDist) {
    WeightedShowdownEval *evaluator = new WeightedShowdownEval(calc, 700);
    vector<double> weights({0,0,0,0,0,0});
    evaluator->apply_exp(weights);

    //for( int i = 0; i < weights.size(); ++i)
        //std::cout << weights[i] << "\n";
    CHECK_EQUAL(1,weights[0]);
    CHECK_CLOSE(0.00674,weights[5],0.00001);
  }

  TEST_FIXTURE(TestStartup, TestApplyGaussDist) {
    WeightedShowdownEval *evaluator = new WeightedShowdownEval(calc, 700);
    vector<double> weights({0,0,0,0,0,0});
    evaluator->apply_gauss(weights);

    //for( int i = 0; i < weights.size(); ++i)
        //std::cout << weights[i] << "\n";
    CHECK_CLOSE(0.398,weights[0],0.001);
    CHECK_CLOSE(0.0,weights[5],0.001);
  }

  void print_weights(vector<double> weights){
    // normalize
    double sum = 0;
    for(unsigned i = 0; i  < weights.size(); ++i){
        sum += weights[i];
    }
    
    for(unsigned i = 0; i  < weights.size(); ++i){
        std::cout << i << ": " << std::string((int)(100*(weights[i] / sum)),'#') << "\n";
    }
  }

  //TEST_FIXTURE(TestStartup, TestSimulateWeightedShowdownEvalModifiyRange) {
    //WeightedShowdownEval *evaluator = new WeightedShowdownEval(calc, 700);

    //Hand hand_a("AhAs");
    //Hand hand_b("KhKs");
    //Hand hand_c("QhQs");
    //Hand hand_d("JhJs");
    //Hand hand_e("ThTs");
    //Hand hand_f("9h9s");
    //BucketCollection buckets(6);
    //buckets.buckets[0].push_back(BucketHand(hand_a, 1));
    //buckets.buckets[1].push_back(BucketHand(hand_b, 1));
    //buckets.buckets[2].push_back(BucketHand(hand_c, 1));
    //buckets.buckets[3].push_back(BucketHand(hand_d, 1));
    //buckets.buckets[4].push_back(BucketHand(hand_e, 1));
    //buckets.buckets[5].push_back(BucketHand(hand_f, 1));
    //WeightedBucketHandlist *handlist =
        //new WeightedBucketHandlist(buckets, vector<double>({1, 1, 1, 1,1,1}));

    //ActionSequence seq;
    //seq.append(Action(ActionType::Call),PhaseType::Flop);
    //seq.append(Action(ActionType::Raise),PhaseType::Flop);

    //vector<double> new_weights =
        //evaluator->modify_range(handlist->weights, seq);

    ////print_weights(new_weights);

    ////WeightedBucketHandlist* new_w = new WeightedBucketHandlist(*handlist,new_weights);

    //delete evaluator;
    //delete handlist;
    ////delete new_w;
  //}

  TEST_FIXTURE(TestStartup, TestSimulateWeightedShowdownEval) {
    WeightedShowdownEval *evaluator = new WeightedShowdownEval(calc, 700);

    Hand hand_a("KhKs");
    Hand hand_b("QhQs");
    Hand hand_c("6h6s");
    Hand hand_d("5h5s");
    Hand hand_e("4h4s");
    Hand hand_f("3h3s");
    BucketCollection buckets(6);
    buckets.buckets[0].push_back(BucketHand(hand_a, 1));
    buckets.buckets[1].push_back(BucketHand(hand_b, 1));
    buckets.buckets[2].push_back(BucketHand(hand_c, 1));
    buckets.buckets[3].push_back(BucketHand(hand_d, 1));
    buckets.buckets[4].push_back(BucketHand(hand_e, 1));
    buckets.buckets[5].push_back(BucketHand(hand_f, 1));


    ActionSequence seq2;
    //seq2.append(Action(ActionType::Call),PhaseType::Flop);

    WeightedBucketHandlist *handlist =
        new WeightedBucketHandlist(buckets, vector<double>({1,1,1}), seq2);

    Handlist *single = new SingleHandlist(Hand("7h7d"));
    vector<unsigned> bboard(
        {Card("Kc").card() });
    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                single),
        StatusType::Active);
    FSeat p1(
        FPlayer("p1", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                handlist),
        StatusType::Active);

    vector<FSeat> seats({mark, p1});
    FContextConfig *ccon = new FContextConfig(
        Hand("7h2d"), 2, bboard, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    ActionSequence seq;
    seq.append(Action(ActionType::Call),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seq.append(Action(ActionType::Raise),PhaseType::Flop,0);
    seats[1].player.action_sequence = seq;
    //std::cout << evaluator->simulate(con) << std::endl;
    FContext *con2 = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);
    CHECK(evaluator->simulate(*con) > evaluator->simulate(*con2));

    delete single;
    delete evaluator;
    delete handlist;
  }

  TEST_FIXTURE(TestStartup, TestWeightedShowdownBucketHandNotAssignable) {
    WeightedShowdownEval *evaluator = new WeightedShowdownEval(calc, 700);

    Hand hand_a("KhKs");
    Hand hand_b("QhQs");
    Hand hand_c("6h6s");
    Hand hand_d("5h5s");
    Hand hand_e("4h4s");
    Hand hand_f("3h3s");
    BucketCollection buckets(6);
    buckets.buckets[0].push_back(BucketHand(hand_a, 1));
    buckets.buckets[1].push_back(BucketHand(hand_b, 1));
    buckets.buckets[2].push_back(BucketHand(hand_c, 1));
    buckets.buckets[3].push_back(BucketHand(hand_d, 1));
    buckets.buckets[4].push_back(BucketHand(hand_e, 1));
    buckets.buckets[5].push_back(BucketHand(hand_f, 1));


    ActionSequence seq2;
    //seq2.append(Action(ActionType::Call),PhaseType::Flop);

    WeightedBucketHandlist *handlist =
        new WeightedBucketHandlist(buckets, vector<double>({1,1,1}), seq2);

    Handlist *single = new SingleHandlist(Hand("6h7d"));
    vector<unsigned> bboard(
        {Card("Kc").card()});
    FSeat mark(
        FPlayer("mark", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                single),
        StatusType::Active);
    FSeat p1(
        FPlayer("p1", bb(10),
                vector<bb>({bb(0), bb(0), bb(0), bb(0)}),
                handlist),
        StatusType::Active);

    vector<FSeat> seats({mark, p1});
    FContextConfig *ccon = new FContextConfig(
        Hand("7h2d"), 2, bboard, vector<double>({1}), vector<double>({3}));
    FContext *con = new FContext(
        pot, highest_bet, index_bot, index_utg, index_button, index_active,
        betting_round, phase, seats, Action(ActionType::None, bb(0)), ccon);

    try{
      evaluator->simulate(*con);
      CHECK(true);
    }
    catch(std::exception &e){
      CHECK(false);
    }

    delete single;
    delete evaluator;
    delete handlist;
  }
}
