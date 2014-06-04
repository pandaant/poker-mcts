#include <vector>
#include <iostream>
#include <UnitTest++.h>
#include <histogramm.hpp>
#include <phase_histogramm.hpp>
#include <round_histogramm.hpp>
#include <model_selector.hpp>
#include <fcontext.hpp>
#include <fplayer.hpp>
#include <fcontext_config.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/random_handlist.hpp>
#include <freedom.hpp>
#include <showdown_eval.hpp>
#include <opponent_node.hpp>
#include <betamt_ev_ratio_selector.hpp>
#include <final_move_selector.hpp>
#include <ecalc/handranks.hpp>

SUITE(SelectionTests) {

  using namespace std;
  using namespace freedom;
  typedef PhaseHistogramm PH;
  typedef RoundHistogramm RH;

  using ecalc::Handlist;
  using ecalc::Handranks;
  using ecalc::RandomHandlist;
  using ecalc::SingleHandlist;

  typedef typename ISelectionStrategy<FContext, FConfig>::sstrategy_t
  sstrategy_t;
  typedef typename INode<FContext, FConfig>::node_t node_t;

  ecalc::Handranks handranks("../../../bin/data/handranks.dat");

  struct ComplexSetup {
    FConfig *config;
    FContextConfig *cconfig;
    vector<FPlayer> players;
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
    double rake_factor;

    // fconfig
    ECalc *calc;
    int max_betting_rounds;
    double time_s;
    int ecalc_nb_samples;
    sstrategy_t *decision_selection_strat;
    sstrategy_t *opponent_selection_strat;
    IBackpropagationStrategy *decision_backprop_strat;
    IBackpropagationStrategy *opponent_backprop_strat;
    ISimulationStrategy<FContext> *sim_strat;
    RootNode<FContext, FConfig, DecisionNode> *root;

    ComplexSetup()
        : players(
              {FPlayer("mark", bb(10), vector<bb>({bb(5), bb(0), bb(0), bb(0)}),
                       StatusType::Active),
               FPlayer("simon", bb(10),
                       vector<bb>({bb(5), bb(0), bb(0), bb(0)}),
                       StatusType::Active)}),
          random(new RandomHandlist()) {
      pot = bb(10);
      highest_bet = bb(0);
      index_bot = 0;
      index_utg = 0;
      index_button = 0;
      index_active = 0;
      betting_round = 0;
      rake_factor = 0;
      board = vector<unsigned>();
      phase = PhaseType::Preflop;
      bot_hl = new SingleHandlist(Hand("AhAs"));

      players[0].handlist = bot_hl;
      players[1].handlist = random;
      // players[2].handlist = random;

      cconfig = new FContextConfig(Hand("AhAs"), 2, board, vector<double>({1}),
                                   vector<double>({3}), rake_factor);
      context = new FContext(pot, highest_bet, index_bot, index_utg,
                             index_button, index_active, betting_round, phase,
                             players, Action(ActionType::None, bb(0)), cconfig);

      calc = new ECalc(&handranks, 0);
      time_s = 1;
      ecalc_nb_samples = 7;
      decision_selection_strat = new MaxValueSelector<FContext, FConfig>();
      opponent_selection_strat = new MaxValueSelector<FContext, FConfig>();
      decision_backprop_strat = new AvgBackpropagationStrategy();
      opponent_backprop_strat = new AvgBackpropagationStrategy();
      sim_strat = new ShowdownEval(calc, ecalc_nb_samples);

      config = new FConfig(time_s, ecalc_nb_samples, calc, sim_strat,
                           decision_backprop_strat, opponent_backprop_strat,
                           decision_selection_strat, opponent_selection_strat,
                           decision_selection_strat, false);
      root = new RootNode<FContext, FConfig, DecisionNode>(*context, config);
    }

    // TODO
    ~ComplexSetup() {
      /*        delete context;
                delete cconfig;
                delete bot_hl;
                delete random;

                delete decision_selection_strat;
                delete opponent_selection_strat;
                delete decision_backprop_strat;
                delete opponent_backprop_strat;
                delete config;*/
    }
  };

  struct Setup {
    Histogramm test_hist;
    ComplexSetup setp;

    Setup() {
      setp = ComplexSetup();
      // preflop
      // first
      // check/bet 60% / 10%
      // fold/call/raise 60% / 30% / 10%

      // second
      // fold/call/raise 80% / 15% / 5%
      vector<RH> pfrounds({RH(600, 600, 300, 0, 100), RH(800, 0, 150, 0, 50)});

      // flop
      // first
      // check/bet 40% / 60%
      // fold/call/raise 50% / 35% / 15%

      // second
      // fold/call/raise 60% / 25% / 15%
      vector<RH> frounds(
          {RH(500, 400, 350, 600, 150), RH(600, 0, 250, 0, 150)});

      PH preflop(pfrounds);
      vector<PH> phases({pfrounds, frounds});
      test_hist = Histogramm("default", phases);
    }
    ~Setup() {}
  };

  // TEST_FIXTURE(Setup, TestGetNormalizedActionProbabilities) {
  // ModelSelector select(&test_hist);

  ////    select.select(n);
  //}

  TEST_FIXTURE(Setup, TestGetNormalizedActionProbabilitiesXB) {
    ModelSelector<FContext, FConfig> select;

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(0.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.players[0].model = "default";
    setp.players[1].model = "default";

    setp.config->models["default"] = &test_hist;

    setp.context = new FContext(bb(1.5), bb(0), setp.index_bot, setp.index_utg,
                                setp.index_button, setp.index_active, 0,
                                PhaseType::Flop, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    vector<PAction<FContext, FConfig>> actions =
        select.normalized_probabilities(dn);

    CHECK_EQUAL(ActionType::Check, actions[0].action.action);
    CHECK_CLOSE(0.4, actions[0].prob, 0.01);

    CHECK_EQUAL(ActionType::Raise, actions[1].action.action);
    CHECK_CLOSE(0.6, actions[1].prob, 0.01);

    delete dn;
  }

  // This will fail because one case is not handled properly on preflop
  // look @ RoundHistogramm::get_action_probabilities

  TEST_FIXTURE(Setup, TestGetNormalizedActionProbabilitiesXR) {
    ModelSelector<FContext, FConfig> select;

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(0.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.players[0].model = "default";
    setp.players[1].model = "default";

    setp.config->models["default"] = &test_hist;

    setp.context = new FContext(bb(1.5), bb(1), setp.index_bot, setp.index_utg,
                                setp.index_button, setp.index_active, 1,
                                setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    vector<PAction<FContext, FConfig>> actions =
        select.normalized_probabilities(dn);

    //        for(auto a:actions) std::cout << a.action.to_str() << " " <<
    // a.prob << std::endl;

    //        CHECK_EQUAL(ActionType::Check,actions[0].action.action);
    //        CHECK_CLOSE(0.8571,actions[0].prob, 0.01);
    //
    //        CHECK_EQUAL(ActionType::Raise,actions[1].action.action);
    //        CHECK_CLOSE(0.1428,actions[1].prob, 0.01);

    delete dn;
  }

  TEST_FIXTURE(Setup, TestGetNormalizedActionProbabilitiesFCR) {
    ModelSelector<FContext, FConfig> select;

    setp.config->models["default"] = &test_hist;

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.players[0].model = "default";
    setp.players[1].model = "default";

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();
    vector<PAction<FContext, FConfig>> actions =
        select.normalized_probabilities(dn);

    CHECK_EQUAL(ActionType::Fold, actions[0].action.action);
    CHECK_CLOSE(0.8, actions[0].prob, 0.01);

    CHECK_EQUAL(ActionType::Call, actions[1].action.action);
    CHECK_CLOSE(0.15, actions[1].prob, 0.01);

    CHECK_EQUAL(ActionType::Raise, actions[2].action.action);
    CHECK_CLOSE(0.05, actions[2].prob, 0.01);

    delete dn;
  }

  TEST_FIXTURE(Setup, TestModelSelectorGetDiscreteIndex) {
    ModelSelector<FContext, FConfig> select;

    setp.config->models["default"] = &test_hist;

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.players[0].model = "default";
    setp.players[1].model = "default";

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();
    vector<PAction<FContext, FConfig>> actions =
        select.normalized_probabilities(dn);

    CHECK_EQUAL(ActionType::Fold, actions[0].action.action);
    CHECK_EQUAL(ActionType::Call, actions[1].action.action);
    CHECK_EQUAL(ActionType::Raise, actions[2].action.action);

    vector<double> probs;
    for (auto a : actions)
      probs.push_back(a.prob);

    int count_f = 0;
    int count_c = 0;
    int count_r = 0;
    for (int i = 0; i < 1000; ++i) {
      int s = select.choose_discrete_index(*setp.config->nb_gen(), probs);
      if (ActionType::Fold == actions[s].action.action)
        ++count_f;
      if (ActionType::Call == actions[s].action.action)
        ++count_c;
      if (ActionType::Raise == actions[s].action.action)
        ++count_r;
    }

    int ges = count_f + count_c + count_r;
    CHECK_CLOSE(0.8, ((count_f + 0.0) / ges), 0.01);
    CHECK_CLOSE(0.15, ((count_c + 0.0) / ges), 0.01);
    CHECK_CLOSE(0.05, ((count_r + 0.0) / ges), 0.01);

    delete dn;
  }

  TEST_FIXTURE(Setup, TestModelSelectorSelect) {
    ModelSelector<FContext, FConfig> select;

    setp.config->models["default"] = &test_hist;

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.players[0].model = "default";
    setp.players[1].model = "default";

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    int count_f = 0;
    int count_c = 0;
    int count_r = 0;
    for (int i = 0; i < 1000; ++i) {
      Action s = select.select(dn)->context().last_action;
      if (ActionType::Fold == s.action)
        ++count_f;
      if (ActionType::Call == s.action)
        ++count_c;
      if (ActionType::Raise == s.action)
        ++count_r;
    }

    int ges = count_f + count_c + count_r;
    CHECK_CLOSE(0.8, ((count_f + 0.0) / ges), 0.01);
    CHECK_CLOSE(0.15, ((count_c + 0.0) / ges), 0.01);
    CHECK_CLOSE(0.05, ((count_r + 0.0) / ges), 0.01);

    delete dn;
  }

  TEST_FIXTURE(Setup, TestBetAmtRatioSelector) {
    BetamtEVRatioSelector<FContext, FConfig> select(1);

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    // ratio -> 2
    static_cast<OpponentNode *>(dn->children()[1])->backpropagate(1);
    // ratio -> 1.1111...
    static_cast<OpponentNode *>(dn->children()[2])->backpropagate(5);

    node_t *selected = select.select(dn);

    CHECK_EQUAL(ActionType::Call, selected->context().last_action.action);
  }

  TEST_FIXTURE(Setup, TestBetAmtRatioSelectorThreshold) {
    BetamtEVRatioSelector<FContext, FConfig> select(2.5);

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    // ratio -> 2
    static_cast<OpponentNode *>(dn->children()[1])->backpropagate(1);
    // ratio -> 1.1111...
    static_cast<OpponentNode *>(dn->children()[2])->backpropagate(5);

    node_t *selected = select.select(dn);

    CHECK_EQUAL(ActionType::Fold, selected->context().last_action.action);
  }

  TEST_FIXTURE(Setup, TestFinalMoveSelector) {
    FinalMoveSelector<FContext, FConfig> selector(1, 3, 2);

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    dn->children()[1]->backpropagate(1);
    dn->children()[2]->backpropagate(5);

    node_t *selected = selector.select(dn);

    CHECK_EQUAL(ActionType::Raise, selected->context().last_action.action);
  }

  TEST_FIXTURE(Setup, TestFinalMoveSelectorAmtEvRatioToLow) {
    FinalMoveSelector<FContext, FConfig> selector(1.5, 3, 2);

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    dn->children()[1]->backpropagate(0.5);
    dn->children()[2]->backpropagate(-1);

    node_t *selected = selector.select(dn);

    CHECK_EQUAL(ActionType::Fold, selected->context().last_action.action);
  }

  TEST_FIXTURE(Setup, TestFinalMoveSelectorBigRaise) {
    FinalMoveSelector<FContext, FConfig> selector(1, 3, 2);

    setp.players = vector<FPlayer>(
        {FPlayer("mark", bb(10), vector<bb>({bb(1), bb(0), bb(0), bb(0)}),
                 StatusType::Active),
         FPlayer("simon", bb(10), vector<bb>({bb(1.5), bb(0), bb(0), bb(0)}),
                 StatusType::Active)});

    setp.context = new FContext(bb(2.5), bb(1.5), setp.index_bot,
                                setp.index_utg, setp.index_button,
                                setp.index_active, 1, setp.phase, setp.players,
                                Action(ActionType::None, bb(0)), setp.cconfig);

    DecisionNode *dn =
        new DecisionNode(setp.context->clone(), setp.config, setp.root);
    dn->expand();

    dn->children()[1]->backpropagate(0.5);
    dn->children()[2]->backpropagate(15);

    node_t *selected = selector.select(dn);

    CHECK_EQUAL(ActionType::Raise, selected->context().last_action.action);
    CHECK_EQUAL(bb(9), selected->context().last_action.amount);
  }
}
