#include <UnitTest++.h>
#include <chrono>
#include <string.h>
#include <range_predictor.hpp>
#include <ehs2_range_predictor.hpp>
#include <ecalc/handranks.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/random_handlist.hpp>
#include <bucket_hand.hpp>
#include <balanced_bucketizer.hpp>
#include <exponential_bucketizer.hpp>
#include <iostream>
#include <ecalc/ecalc.hpp>
#include <histogramm.hpp>
#include <poker/action_sequence.hpp>

SUITE(RangePredictorTests) {
  using namespace Freedom5;
  using namespace poker;

  ecalc::Handranks handranks("../../../bin/data/handranks.dat");

  using ecalc::Handranks;
  using ecalc::ECalc;
  using ecalc::SingleHandlist;
  using ecalc::RandomHandlist;

  struct Setup {
    ECalc *calc;
    int ecalc_nb_samples;
    Histogramm histogramm_model;
    Histogramm histogramm_model_complete;
    typedef PhaseHistogramm PH;
    typedef RoundHistogramm RH;

    Setup() {
      calc = new ECalc(&handranks, 0);

      // preflop
      // first
      // check/raise 60% / 40%
      // fold/call/raise 60% / 30% / 10%

      // second
      // fold/call/raise 80% / 15% / 5%
      vector<RH> pfrounds(
          {RH(600, 600, 300, 400, 100), RH(800, 0, 150, 0, 50)});

      // flop
      // first
      // check/bet 40% / 60%
      // fold/call/raise 50% / 35% / 15%

      // second
      // fold/call/raise 60% / 25% / 15%
      vector<RH> frounds(
          {RH(500, 400, 350, 600, 150), RH(600, 0, 250, 0, 150)});

      setup_model2();

      PH preflop(pfrounds);
      vector<PH> phases({pfrounds, frounds});
      histogramm_model = Histogramm("test", phases);
    }

    void setup_model2() {
      /*
      * preflop
      *
      * first
      * check/bet 80% / 20%
      * fold/call/raise 50% / 35% / 15%
      *
      * second
      * fold/call/raise 40% / 35% / 25%
      *
      * third
      * fold/call/raise 20% / 45% / 35%
      *
      * fourth
      * fold/call/raise 10% / 50% / 40%
      *
      * fifth
      * fold/call/raise 10% / 90% / 0%
      */
      vector<RH> pfrounds({RH(500, 800, 350, 200, 150), RH(800, 0, 150, 0, 50),
                           RH(200, 0, 450, 0, 350), RH(100, 0, 500, 0, 400),
                           RH(100, 0, 900, 0, 0)});

      /*
      * flop
      *
      * first
      * check/bet 50% / 50%
      * fold/call/raise 30% / 40% / 30%
      *
      * second
      * fold/call/raise 40% / 35% / 25%
      *
      * third
      * fold/call/raise 20% / 55% / 15%
      *
      * fourth
      * fold/call/raise 10% / 80% / 10%
      *
      * fifth
      * fold/call/raise 10% / 90% / 0%
      */
      vector<RH> frounds({RH(500, 500, 400, 500, 300), RH(400, 0, 350, 0, 250),
                          RH(200, 0, 550, 0, 150), RH(100, 0, 800, 0, 100),
                          RH(100, 0, 900, 0, 0)});

      /*
      * turn
      *
      * first
      * check/bet 50% / 50%
      * fold/call/raise 30% / 40% / 30%
      *
      * second
      * fold/call/raise 40% / 35% / 25%
      *
      * third
      * fold/call/raise 20% / 55% / 15%
      *
      * fourth
      * fold/call/raise 10% / 80% / 10%
      *
      * fifth
      * fold/call/raise 10% / 90% / 0%
      */
      vector<RH> trounds({RH(500, 500, 400, 500, 300), RH(400, 0, 350, 0, 250),
                          RH(200, 0, 550, 0, 150), RH(100, 0, 800, 0, 100),
                          RH(100, 0, 900, 0, 0)});

      /*
      * river
      *
      * first
      * check/bet 50% / 50%
      * fold/call/raise 30% / 40% / 30%
      *
      * second
      * fold/call/raise 40% / 35% / 25%
      *
      * third
      * fold/call/raise 20% / 55% / 15%
      *
      * fourth
      * fold/call/raise 10% / 80% / 10%
      *
      * fifth
      * fold/call/raise 10% / 90% / 0%
      */
      vector<RH> rrounds({RH(500, 500, 400, 500, 300), RH(400, 0, 350, 0, 250),
                          RH(200, 0, 550, 0, 150), RH(100, 0, 800, 0, 100),
                          RH(100, 0, 900, 0, 0)});

      vector<PH> phases({pfrounds, frounds, trounds, rrounds});
      histogramm_model_complete = Histogramm("default", phases);
    }

    ~Setup() {
      delete calc;
    }
  };

  /**
   * Completete test
   * calculate handrange for a model
   **/
  TEST_FIXTURE(Setup, TestPredictRange) {
    RangePredictor pr(calc, 10, Threshold(2, 2),
                      vector<int>({663, 20, 20, 20}));

    /* create action sequence for preflop
     * preflop: Bet 10bb
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Bet, bb(10)), PhaseType::Preflop, 0);

    // a board and deadcards
    vector<unsigned> board;
    vector<unsigned> dead;
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);

    CHECK_EQUAL(532, range.size());
  }

  // test 2 actions over two phases
  TEST_FIXTURE(Setup, TestPredictRange2action) {
    RangePredictor pr(calc, 300, Threshold(2, 2),
                      vector<int>({663, 20, 20, 20}));

    /* create action sequence for preflop
     * preflop: Bet 10bb
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Bet, bb(10)), PhaseType::Preflop, 0);
    actions.append(Action(ActionType::Bet, bb(10)), PhaseType::Flop, 0);

    // a board and deadcards
    vector<unsigned> board(
        {Card("2h").card(), Card("4s").card(), Card("5d").card()});
    vector<unsigned> dead;
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);

    CHECK_EQUAL(288, range.size());
  }

  TEST_FIXTURE(Setup, TestPredictRangeTheThird) {
    RangePredictor pr(calc, 10, Threshold(2, 2), vector<int>({10, 20, 20}));

    /* create action sequence for preflop
     * preflop: call 10bb
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Call, bb(10)), PhaseType::Preflop, 0);

    // a board and deadcards
    vector<unsigned> board(
        {Card("Ah").card(), Card("7d").card(), Card("2d").card()});
    vector<unsigned> dead;
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);

    CHECK_EQUAL(354, range.size());
  }

  TEST_FIXTURE(Setup, TestPredictRangeTheFourth) {
    RangePredictor pr(calc, 10, Threshold(2, 2), vector<int>({20, 20, 20, 20}));

    /* create action sequence for preflop
     * preflop: call 10bb
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Call, bb(1)), PhaseType::Preflop, 0);

    // a board and deadcards
    vector<unsigned> board(
        {Card("Ah").card(), Card("7d").card(), Card("2d").card()});
    vector<unsigned> dead({51, 49});
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);

    for (int i = 0; i < range.size(); ++i) {
      CHECK(range[i].lowcard().card() != 0 &&
            range[i].highcard().card() != 0);
    }
    CHECK_EQUAL(1128, range.size());
  }

  /**
   * Bug test. calculated range of player empty
   */
  TEST_FIXTURE(Setup, TestBugEmptyRange) {
    RangePredictor pr(calc, 1000, Threshold(2, 2),
                      vector<int>({6, 10, 10, 10}));

    /* create action sequence for preflop
     * preflop: call 4bb
     * flop: check 0bb
     * turn: raise 6bb, call 10bb
     * river: raise 12bb
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Call, bb(4)), PhaseType::Preflop, 1);
    actions.append(Action(ActionType::Check, bb(0)), PhaseType::Flop, 0);
    actions.append(Action(ActionType::Raise, bb(6)), PhaseType::Turn,1);
    actions.append(Action(ActionType::Call, bb(10)), PhaseType::Turn,3);
    actions.append(Action(ActionType::Raise, bb(12)), PhaseType::River,0);
    actions.append(Action(ActionType::Raise, bb(12)), PhaseType::River,2);

    // a board and deadcards
    vector<unsigned> board({22, 1, 31, 19, 49});
    vector<unsigned> dead({51, 44});

    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model_complete, board, dead);
    CHECK(0 != range.size());
  }

  /**
   * check that there exists no configuration that returns an empty range.
   */
  TEST_FIXTURE(Setup, TestRangeCannotBeEmpty) {
    RangePredictor pr(calc, 10, Threshold(2, 2), vector<int>({10, 20, 20, 20}),
                      50);

    ActionSequence actions;
    actions.append(Action(ActionType::Call, bb(10, 0)), PhaseType::Preflop,0);
    actions.append(Action(ActionType::Call, bb(10, 0)), PhaseType::Preflop,0);
    actions.append(Action(ActionType::Call, bb(10, 0)), PhaseType::Preflop,0);
    // each call reduces the range with factor 0.3.
    // so after 3 calls the range should be 36

    // a board and deadcards
    vector<unsigned> board;
    vector<unsigned> dead;
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);
    CHECK(range.size() >= 50);
  }

  TEST_FIXTURE(Setup, TestTopNHands) {
    RangePredictor pr(calc, 10, Threshold(2, 2), vector<int>({2, 20, 20, 20}),
                      5);

    vector<BucketHand> collection(
        {BucketHand(Hand("AhAs"), 10), BucketHand(Hand("KhKs"), 9),
         BucketHand(Hand("QhQs"), 8), BucketHand(Hand("JhQs"), 7),
         BucketHand(Hand("ThQs"), 6), BucketHand(Hand("9hQs"), 5),
         BucketHand(Hand("8hQs"), 4), BucketHand(Hand("7hQs"), 3),
         BucketHand(Hand("6hQs"), 2), BucketHand(Hand("5hQs"), 1)});

    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(2, collection);
    vector<BucketHand> range = pr.top_n_hands(buckets, 2);

    CHECK_EQUAL(5, range.size());
  }

  TEST_FIXTURE(Setup, TestPredictRangeActionZeroProbability) {
    RangePredictor pr(calc, 10, Threshold(2, 2), vector<int>({10, 20, 20, 20}),
                      5, 0.2);
    /* create action sequence for preflop
     * preflop:call 10bb in betting round 2
     */
    ActionSequence actions;
    actions.append(Action(ActionType::Call, bb(10)), PhaseType::Preflop, 2);

    // a board and deadcards
    vector<unsigned> board(
        {Card("Ah").card(), Card("7d").card(), Card("2d").card()});
    vector<unsigned> dead;
    vector<Hand> range = pr.predict_range<BalancedBucketizer>(
        actions, &histogramm_model, board, dead);
    /**
     * i calculated 132 hands per bucket ( 1326/10 ) but it seems
     * the bucketizer ceils the float to 133. from that follows that
     * only 2 instead of 3 buckets are needed. */
    CHECK_EQUAL(236, range.size());
  }

  TEST_FIXTURE(Setup, TestBoardByPhase) {
    RangePredictor pr(calc, 10);

    vector<unsigned> board(
        {Card("Ah").card(), Card("7d").card(), Card("2d").card()});
    CHECK_EQUAL(0, pr.board_by_phase(board, PhaseType::Preflop).size());
    CHECK_EQUAL(3, pr.board_by_phase(board, PhaseType::Flop).size());
  }

  TEST_FIXTURE(Setup, TestDeadByPhase) {
    RangePredictor pr(calc, 10);

    vector<unsigned> board(
        {Card("Ah").card(), Card("7d").card(), Card("2d").card()});
    CHECK_EQUAL(
        3, pr.dead_by_phase(board, vector<unsigned>(), PhaseType::Preflop).size());
    CHECK_EQUAL(0,
                pr.dead_by_phase(board, vector<unsigned>(), PhaseType::Flop).size());
    CHECK_EQUAL(
        Card("Ah").card(),
        pr.dead_by_phase(board, vector<unsigned>(), PhaseType::Preflop).at(0));
    CHECK_EQUAL(
        Card("2d").card(),
        pr.dead_by_phase(board, vector<unsigned>(), PhaseType::Preflop).at(2));
  }

  TEST_FIXTURE(Setup, TestUpdateEquities) {
    RangePredictor pr(calc, 10);

    vector<BucketHand> overwrite(
        {BucketHand(Hand("AhAs"), 0), BucketHand(Hand("KhKs"), 0),
         BucketHand(Hand("QhQs"), 0), BucketHand(Hand("JhQs"), 0),
         BucketHand(Hand("ThQs"), 0), BucketHand(Hand("9hQs"), 0),
         BucketHand(Hand("8hQs"), 0), BucketHand(Hand("7hQs"), 0),
         BucketHand(Hand("6hQs"), 0), BucketHand(Hand("5hQs"), 0)});

    vector<BucketHand> source(
        {BucketHand(Hand("AhAs"), 1), BucketHand(Hand("KhKs"), 1),
         BucketHand(Hand("QhQs"), 1), BucketHand(Hand("JhQs"), 1),
         BucketHand(Hand("ThQs"), 1), BucketHand(Hand("6hQs"), 1),
         BucketHand(Hand("5hQs"), 1)});

    overwrite = pr.update_equities(overwrite, source);

    //    for(auto o:overwrite) std::cout << o.to_str() << " " << o.equity <<
    // std::endl;

    // 3 hands less
    CHECK_EQUAL(7, overwrite.size());
    // no more zero equities
    for (auto i : overwrite)
      CHECK_EQUAL(1, i.equity);
  }

  TEST_FIXTURE(Setup, TestCalculateUpperBound) {
    RangePredictor pr(calc, 10);

    Action action(ActionType::Bet, bb(10));
    PhaseType::Enum phase = PhaseType::Preflop;

    vector<BucketHand> hands = pr.hand_combinations(vector<unsigned>());
    pr.calculate_handstrengths(hands, vector<unsigned>(), vector<unsigned>());
    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(663, hands);
    // there should be 2 hands in every bucket.

    int upper_bound =
        pr.calculate_upper_bound(action, phase, 0, &histogramm_model, buckets);

    CHECK_EQUAL(0, upper_bound);

    // test with action call in bettinground 1
    upper_bound =
        pr.calculate_upper_bound(Action(ActionType::Call, bb(10)), phase, 1,
                                 &histogramm_model, buckets);

    CHECK_EQUAL(99, upper_bound);
  }

  TEST_FIXTURE(Setup, TestCalculateLowerBound) {
    RangePredictor pr(calc, 10);

    Action action(ActionType::Bet, bb(10));
    PhaseType::Enum phase = PhaseType::Preflop;

    vector<BucketHand> hands = pr.hand_combinations(vector<unsigned>());
    pr.calculate_handstrengths(hands, vector<unsigned>(), vector<unsigned>());
    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(663, hands);
    // there should be 2 hands in every bucket.

    int upper_bound =
        pr.calculate_upper_bound(action, phase, 0, &histogramm_model, buckets);

    int lower_bound = pr.calculate_lower_bound(action, phase, 0, &histogramm_model,
                                               buckets, upper_bound);

    // 265 because 531 hands and 2 per bucket and - 1 because its the index
    CHECK_EQUAL(265, lower_bound);

    // call action
    action = Action(ActionType::Call, bb(3));
    upper_bound =
        pr.calculate_upper_bound(action, phase, 1, &histogramm_model, buckets);

    lower_bound = pr.calculate_lower_bound(action, phase, 1, &histogramm_model,
                                           buckets, upper_bound);

    CHECK_EQUAL(99, upper_bound);
    CHECK_EQUAL(198, lower_bound);
  }

  TEST_FIXTURE(Setup, TestCalculateUpperLowerBoundWeakActions) {
    RangePredictor pr(calc, 10, Threshold(2, 3));

    // amount is smaller than threshold
    Action action(ActionType::Bet, bb(1));
    PhaseType::Enum phase = PhaseType::Preflop;

    vector<BucketHand> hands = pr.hand_combinations(vector<unsigned>());
    pr.calculate_handstrengths(hands, vector<unsigned>(), vector<unsigned>());
    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(663, hands);
    // there should be 2 hands in every bucket.

    int upper_bound =
        pr.calculate_upper_bound(action, phase, 0, &histogramm_model, buckets);

    int lower_bound = pr.calculate_lower_bound(action, phase, 0, &histogramm_model,
                                               buckets, upper_bound);

    CHECK_EQUAL(0, upper_bound);
    CHECK_EQUAL(662, lower_bound);

    // weak call action flop
    action = Action(ActionType::Call, bb(2));
    upper_bound =
        pr.calculate_upper_bound(action, phase, 1, &histogramm_model, buckets);

    lower_bound = pr.calculate_lower_bound(action, phase, 1, &histogramm_model,
                                           buckets, upper_bound);

    CHECK_EQUAL(0, upper_bound);
    CHECK_EQUAL(662, lower_bound);
  }

  /* bugcheck */
  TEST_FIXTURE(Setup, TestOverLoadedNoVirtualFunctionCallEHS2) {
    RangePredictor *pr = new RangePredictor(calc, 1000);
    RangePredictor *pr2 = new EHS2RangePredictor(calc, 1000);

    vector<Handlist *> lists(
        {new SingleHandlist(Hand("AhAs")), new RandomHandlist()});

    vector<unsigned> board(0);
    vector<unsigned> deadcards(0);

    double eq = pr->equity_first(lists, board, deadcards, calc);
    double eq2 = pr2->equity_first(lists, board, deadcards, calc);

    CHECK(fabs(eq - eq2) > 0.1);
  }

  TEST_FIXTURE(Setup, TestEquityFirstEHS2) {
    EHS2RangePredictor pr(calc, 1150);
    vector<Handlist *> lists(
        {new SingleHandlist(Hand("AhAs")), new RandomHandlist()});

    vector<unsigned> board(0);
    vector<unsigned> deadcards(0);

    double eq = pr.equity_first(lists, board, deadcards, calc);
    // equity should be a little less than without ehs2
    CHECK_CLOSE(0.73, eq, 0.01);
  }

  TEST_FIXTURE(Setup, TestEquityFirst) {
    RangePredictor pr(calc, 1500);
    vector<Handlist *> lists(
        {new SingleHandlist(Hand("AhAs")), new RandomHandlist()});

    vector<unsigned> board(0);
    vector<unsigned> deadcards(0);

    double eq = pr.equity_first(lists, board, deadcards, calc);
    CHECK_CLOSE(0.85204, eq, 0.01);
  }

  TEST_FIXTURE(Setup, TestCalcHandstrength) {
    RangePredictor pr(calc, 1500);

    vector<unsigned> board(0);
    vector<unsigned> deadcards(0);

    vector<BucketHand> hands({BucketHand(Hand("AhAs"), 0),
                              BucketHand(Hand("KhKs"), 0),
                              BucketHand(Hand("QhQs"), 0)});

    pr.calculate_handstrengths(hands, board, deadcards);

    CHECK_EQUAL(3, hands.size());

    CHECK_CLOSE(0.85, hands[0].equity, 0.05);
    CHECK_CLOSE(0.82, hands[1].equity, 0.05);
    CHECK_CLOSE(0.799, hands[2].equity, 0.05);
  }

  TEST_FIXTURE(Setup, TestCalcHandstrengthDeadcards) {
    RangePredictor pr(calc, 1500);

    vector<unsigned> board(0);
    vector<unsigned> deadcards({Card("Ac").card(), Card("Kc").card()});

    vector<BucketHand> hands({BucketHand(Hand("AhAs"), 0),
                              BucketHand(Hand("KhKs"), 0),
                              BucketHand(Hand("QhQs"), 0)});

    pr.calculate_handstrengths(hands, board, deadcards);

    CHECK_EQUAL(3, hands.size());

    CHECK_CLOSE(0.836, hands[0].equity, 0.05);
    CHECK_CLOSE(0.818, hands[1].equity, 0.05);
    CHECK_CLOSE(0.818, hands[2].equity, 0.05);
    // with deadcards, KhKs == QhQs equity wise
  }

  TEST_FIXTURE(Setup, TestCalcHandstrengthBoardAndDeadcards) {
    RangePredictor pr(calc, 1000);

    vector<unsigned> board(
        {Card("7h").card(), Card("Jc").card(), Card("2d").card()});

    vector<unsigned> deadcards({Card("Ac").card(), Card("Kc").card()});

    vector<BucketHand> hands({BucketHand(Hand("AhAs"), 0),
                              BucketHand(Hand("KhKs"), 0),
                              BucketHand(Hand("QhQs"), 0)});

    pr.calculate_handstrengths(hands, board, deadcards);

    CHECK_EQUAL(3, hands.size());

    CHECK_CLOSE(0.864, hands[0].equity, 0.05);
    CHECK_CLOSE(0.853, hands[1].equity, 0.05);
    CHECK_CLOSE(0.848, hands[2].equity, 0.05);
  }

  TEST_FIXTURE(Setup, TestBalancedBucketizer) {
    typedef BucketCollection::Bucket Bucket;
    RangePredictor pr(calc, 1000);

    vector<unsigned> board, dead;

    vector<BucketHand> hands = pr.hand_combinations(dead);
    pr.calculate_handstrengths(hands, board, dead);

    CHECK_EQUAL(1326, hands.size());

    CHECK_CLOSE(0.503, hands[0].equity, 0.05);
    CHECK_CLOSE(0.852, hands[1325].equity, 0.05);

    std::sort(hands.begin(), hands.end());

    // for(auto i : hands )
    // std::cout << i.to_str() << " " << i.equity << std::endl;

    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(663, hands);

     //balanced bucketizer should fill all buckets equally if we use 663
    //buckets,
     //each bucket should contain 2 hands
    
    for (auto b : buckets.buckets)
      CHECK_EQUAL(2, b.size());

     //Bug which caused the balanced bucketizer to create a hand to much which
    //was wrong and
     //invalid.
    
    BalancedBucketizer buckbuck2;
    BucketCollection buckets2 = buckbuck2.map_hands(20, hands);

    vector<BucketHand> ran = buckets2.hand_bucket_range(19, 0);
    CHECK(ran[ran.size() - 1].highcard().card() != 0 &&
          ran[ran.size() - 1].lowcard().card() != 0);
  }

  TEST_FIXTURE(Setup, TestMoreBucketsThanHands) {
    RangePredictor pr(calc, 1000);

    vector<BucketHand> hands(
        {BucketHand(Hand("AhAs"), 0), BucketHand(Hand("KhKs"), 0),
         BucketHand(Hand("QhQs"), 0), BucketHand(Hand("JhQs"), 0),
         BucketHand(Hand("ThQs"), 0), BucketHand(Hand("9hQs"), 0),
         BucketHand(Hand("8hQs"), 0), BucketHand(Hand("7hQs"), 0),
         BucketHand(Hand("6hQs"), 0), BucketHand(Hand("5hQs"), 0)});

    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(100, hands);

    CHECK(buckets.count_hands() > 0);
  }

  TEST_FIXTURE(Setup, TestHandCombinations) {
    RangePredictor pr(calc, 1000);

    vector<BucketHand> hands = pr.hand_combinations(vector<unsigned>());

    CHECK_EQUAL(1326, hands.size());

    CHECK_EQUAL("2d2c", hands[0].to_str());
    CHECK_EQUAL("AsAh", hands[1325].to_str());

    vector<unsigned> dead({Card("Ah").card(), Card("Qh").card()});

    hands = pr.hand_combinations(dead);
    CHECK_EQUAL(1225, hands.size());
  }

  TEST_FIXTURE(Setup, TestCombinationFixDeletingWrongCards) {
    RangePredictor pr(calc, 1000);

    vector<unsigned> dead({Card("Ah").card(), Card("Qh").card()});

    vector<BucketHand> hands = pr.hand_combinations(dead);
    CHECK_EQUAL(1225, hands.size());

    // check some impossible hands
    for (auto o : hands) {
      CHECK(o.to_str() != "QhQc");
      CHECK(o.to_str() != "AhQc");
      CHECK(o.to_str() != "QsQh");
      //   std::cout << o.to_str() << std::endl;
    }
  }

  TEST_FIXTURE(Setup, TestBucketCollection) {
    typedef BucketCollection::Bucket Bucket;

    vector<BucketHand> hands(
        {BucketHand(Hand("AhAs"), 0), BucketHand(Hand("KhKs"), 0),
         BucketHand(Hand("QhQs"), 0), BucketHand(Hand("JhQs"), 0),
         BucketHand(Hand("ThQs"), 0), BucketHand(Hand("9hQs"), 0),
         BucketHand(Hand("8hQs"), 0), BucketHand(Hand("7hQs"), 0),
         BucketHand(Hand("6hQs"), 0), BucketHand(Hand("5hQs"), 0)});

    vector<Bucket> buckets(
        {Bucket({hands[0], hands[1]}), Bucket({hands[2], hands[3]}),
         Bucket({hands[4], hands[5]}), Bucket({hands[6], hands[7]}),
         Bucket({hands[8], hands[9]})});

    BucketCollection coll(buckets);

    CHECK_EQUAL(2, coll.get_bucket_index_including_nb_hands(5));
    CHECK_EQUAL(1, coll.get_bucket_index_including_nb_hands(4));
    CHECK_EQUAL(2, coll.get_bucket_index_including_nb_hands(4, 1));
  }

  TEST_FIXTURE(Setup, TestBucketCollectionMinMaxBucketRange) {
    typedef BucketCollection::Bucket Bucket;

    vector<BucketHand> hands(
        {BucketHand(Hand("AhAs"), 0), BucketHand(Hand("KhKs"), 0),
         BucketHand(Hand("QhQs"), 0), BucketHand(Hand("JhQs"), 0),
         BucketHand(Hand("ThQs"), 0), BucketHand(Hand("9hQs"), 0),
         BucketHand(Hand("8hQs"), 0), BucketHand(Hand("7hQs"), 0),
         BucketHand(Hand("6hQs"), 0), BucketHand(Hand("5hQs"), 0)});

    vector<Bucket> buckets(
        {Bucket({hands[0], hands[1]}), Bucket({hands[2], hands[3]}),
         Bucket({hands[4], hands[5]}), Bucket({hands[6], hands[7]}),
         Bucket({hands[8], hands[9]}), });

    BucketCollection coll(buckets);

    CHECK_EQUAL(2, coll.bucket_range(1, 0).buckets.size());
    CHECK_EQUAL(5, coll.bucket_range(4, 0).buckets.size());
    CHECK_EQUAL(1, coll.bucket_range(0, 0).buckets.size());
  }

  TEST_FIXTURE(Setup, TestBucketCollectionHandBucketRange) {
    typedef BucketCollection::Bucket Bucket;

    vector<BucketHand> hands(
        {BucketHand(Hand("AhAs"), 0), BucketHand(Hand("KhKs"), 0),
         BucketHand(Hand("QhQs"), 0), BucketHand(Hand("JhQs"), 0),
         BucketHand(Hand("ThQs"), 0), BucketHand(Hand("9hQs"), 0),
         BucketHand(Hand("8hQs"), 0), BucketHand(Hand("7hQs"), 0),
         BucketHand(Hand("6hQs"), 0), BucketHand(Hand("5hQs"), 0)});

    vector<Bucket> buckets(
        {Bucket({hands[0], hands[1]}), Bucket({hands[2], hands[3]}),
         Bucket({hands[4], hands[5]}), Bucket({hands[6], hands[7]}),
         Bucket({hands[8], hands[9]}), });

    BucketCollection coll(buckets);

    CHECK_EQUAL(4, coll.hand_bucket_range(1, 0).size());
    CHECK_EQUAL(10, coll.hand_bucket_range(4, 0).size());
    CHECK_EQUAL(2, coll.hand_bucket_range(0, 0).size());
  }

  TEST_FIXTURE(Setup, Benchmark2pPf) {
    typedef BucketCollection::Bucket Bucket;
    RangePredictor pr(calc, 1000);

    vector<unsigned> board, dead;

    vector<BucketHand> hands = pr.hand_combinations(dead);
    pr.calculate_handstrengths(hands, board, dead);

    CHECK_EQUAL(1326, hands.size());

    CHECK_CLOSE(0.503, hands[0].equity, 0.05);
    CHECK_CLOSE(0.852, hands[1325].equity, 0.05);

    std::sort(hands.begin(), hands.end());

    BalancedBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(663, hands);

     //balanced bucketizer should fill all buckets equally if we use 663
    //buckets,
     //each bucket should contain 2 hands
    
    for (auto b : buckets.buckets)
      CHECK_EQUAL(2, b.size());
  }

  // test with empty board deadcards
  TEST_FIXTURE(Setup, TestGenerateCacheIdEmpty) {
    RangePredictor pr(calc, 10);

    vector<unsigned> deadcards;
    vector<unsigned> board;

    string id = pr.generate_cache_id(board, deadcards);
    CHECK_EQUAL("", id);
  }

  TEST_FIXTURE(Setup, TestGenerateCacheId) {
    RangePredictor pr(calc, 1000);

    vector<unsigned> deadcards({Card("Ac").card(), Card("Kc").card()});

    vector<unsigned> board(
        {Card("Tc").card(), Card("6c").card(), Card("2d").card()});

    string id = pr.generate_cache_id(board, deadcards);
    CHECK_EQUAL("2d6cTcKcAc", id);
  }

  TEST_FIXTURE(Setup, TestBuildCache) {
    RangePredictor pr(calc, 2000);

    vector<unsigned> deadcards({Card("Ac").card(), Card("Kc").card()});

    vector<unsigned> board({Card("Tc").card(), Card("6c").card(),
                       Card("2d").card(), Card("4d").card(),
                       Card("7d").card()});

    CHECK_EQUAL(0, pr.cache.size());
    pr.build_cache(board, deadcards);

    // check that key exists
    CHECK(pr.cache.find("2d4d6c7dTcKcAc") != pr.cache.end());
    CHECK_EQUAL(990, pr.cache["2d4d6c7dTcKcAc"].size());
  }

  TEST_FIXTURE(Setup, Benchmark2pPfHandstrength) {
    RangePredictor pr(calc, 1000);

    vector<unsigned> board;
    vector<unsigned> deadcards;

    vector<BucketHand> hands = pr.hand_combinations(deadcards);
    auto start = std::chrono::system_clock::now();
    pr.calculate_handstrengths(hands, board, deadcards);
    auto end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start).count();

    std::cout << "Benchmark2pPfHandstrength | 1k samples:\t\t\t"
              << std::chrono::duration_cast<std::chrono::seconds>(end - start)
                     .count() << " s | "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end - start).count() << " ms | "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                     end - start).count() << " micros" << std::endl;
  }

  TEST_FIXTURE(Setup, TestExponentialBucketizer) {
    typedef BucketCollection::Bucket Bucket;
    EHS2RangePredictor *pr = new EHS2RangePredictor(calc, 1200);

    vector<unsigned> board, dead;

    string id = pr->build_cache(board, dead);
    vector<BucketHand> hands = pr->cache[id];

    // for(auto i : hands)
    // std::cout << i.to_str() << std::endl;
    // return;

    ExponentialBucketizer buckbuck;
    BucketCollection buckets = buckbuck.map_hands(25, hands);

    int sum = 0;
    for (int i = 0; i < buckets.nb_buckets; ++i) {
      sum += buckets[i].size();
      // std::cout << "\n" << "hands in bucket " << i << ":\n";
      // for( int a = 0; a < buckets[i].size(); ++a ){
      // std::cout << buckets[i][a].to_str() << ((a+2)%10==0 ? "\n" : " ");
      //}
    }

    CHECK_EQUAL(1326, sum);
    delete pr;
  }

  TEST_FIXTURE(Setup, TestGetNuts) {
    typedef BucketCollection::Bucket Bucket;
    EHS2RangePredictor *pr = new EHS2RangePredictor(calc, 1200);

    vector<unsigned> deadcards({Card("Ac").card(), Card("Kd").card()});

    vector<unsigned> board({Card("8d").card(), Card("3s").card(),
                       Card("2s").card(), Card("Jh").card()});

    // first hand ist strongest
    string strong = pr->get_best_hand(board, deadcards).str169();
    CHECK("JJ" == strong || "88" == strong);

    delete pr;
  }

  TEST_FIXTURE(Setup, TestNutPercentage) {
    EHS2RangePredictor *pr = new EHS2RangePredictor(calc, 1200);

    CHECK_EQUAL(0,pr->calculate_best_hand_percentage(0.1));
    CHECK_CLOSE(0.21333,pr->calculate_best_hand_percentage(0.6), 0.001);
    CHECK_CLOSE(0.01333,pr->calculate_best_hand_percentage(0.4), 0.001);
    CHECK_CLOSE(0.36666,pr->calculate_best_hand_percentage(0.7), 0.001);

    //for(double i = 0.01; i < 1.0; i+= 0.01){
      //std::cout << "p( " << i
                //<< " ) = " << pr->calculate_best_hand_percentage(i)
                //<< std::endl;
    //}

    delete pr;
  }
}
