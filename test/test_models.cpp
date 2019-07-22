#include <iostream>
#include <UnitTest++/UnitTest++.h>
#include <tuple>
#include <histogramm.hpp>
#include <phase_histogramm.hpp>
#include <round_histogramm.hpp>

SUITE(ModelTests) {

  using namespace freedom;
  using std::get;
  using std::tuple;

  typedef PhaseHistogramm PH;
  typedef RoundHistogramm RH;

  struct Setup {

    Setup() {}

    ~Setup() {}
  };

  TEST_FIXTURE(Setup, TestGetActionProbability) {
    // test with only 2 bettingrounds

    // preflop
    // first
    // check/bet 60% / 40%
    // fold/call/raise 60% / 30% / 10%

    // second
    // fold/call/raise 80% / 15% / 5%
    vector<RH> pfrounds(
        { RH(600, 600, 300, 400, 100), RH(800, 0, 150, 0, 50) });

    // flop
    // first
    // check/bet 40% / 60%
    // fold/call/raise 50% / 35% / 15%

    // second
    // fold/call/raise 60% / 25% / 15%
    vector<RH> frounds(
        { RH(500, 400, 350, 600, 150), RH(600, 0, 250, 0, 150) });

    PH preflop(pfrounds);
    vector<PH> phases({ pfrounds, frounds });
    Histogramm test(phases);

    Action action(ActionType::Bet, bb(10));
    double action_prob =
        test.get_action_probability(action, PhaseType::Preflop, 0);
    CHECK_CLOSE(0.4, action_prob, 0.01);

    action = Action(ActionType::Call, bb(10));
    action_prob = test.get_action_probability(action, PhaseType::Preflop, 0);
    CHECK_CLOSE(0.3, action_prob, 0.001);

    // betting round 1 flop
    action = Action(ActionType::Call, bb(10));
    action_prob = test.get_action_probability(action, PhaseType::Flop, 1);
    CHECK_CLOSE(0.25, action_prob, 0.001);

    action = Action(ActionType::Fold, bb(0));
    action_prob = test.get_action_probability(action, PhaseType::Flop, 1);
    CHECK_CLOSE(0.6, action_prob, 0.001);
  }

  TEST_FIXTURE(Setup, TestInit) {
    // test with only 2 bettingrounds

    // first
    // check/bet 60% / 40%
    // fold/call/raise 60% / 30% / 10%

    // second
    // fold/call/raise 80% / 15% / 5%
    vector<RH> pfrounds(
        { RH(600, 600, 300, 400, 100), RH(800, 0, 150, 0, 50) });

    PH preflop(pfrounds);
    vector<PH> phases(1, preflop);
    Histogramm test(phases);

    FContext context = FContext(0, 0, 0, 0, vector<FPlayer>(), NULL);
    tuple<double, double, double> result =
        test.getFoldCallRaiseProbabilities(context);

    CHECK_CLOSE(0.6, get<0>(result), 0.1);
    CHECK_CLOSE(0.3, get<1>(result), 0.1);
    CHECK_CLOSE(0.1, get<2>(result), 0.1);

    tuple<double, double> result1 = test.getCheckBetProbabilities(context);

    CHECK_CLOSE(0.6, get<0>(result1), 0.1);
    CHECK_CLOSE(0.4, get<1>(result1), 0.1);

    context.betting_round = 1;
    result = test.getFoldCallRaiseProbabilities(context);

    CHECK_CLOSE(0.8, get<0>(result), 0.1);
    CHECK_CLOSE(0.15, get<1>(result), 0.1);
    CHECK_CLOSE(0.05, get<2>(result), 0.1);
  }

  TEST_FIXTURE(Setup,TestZeroProbability){
    // preflop
    // first
    // check/bet 60% / 40%
    // fold/call/raise 60% / 30% / 10%

    // second
    // fold/call/raise 80% / 15% / 5%
    vector<RH> pfrounds(
        { RH(600, 600, 300, 400, 100), RH(800, 0, 150, 0, 50) });

    // so betting round 2 should everything be 0
    vector<PH> phases{ pfrounds };
    Histogramm test(phases);

    Action action(ActionType::Call, bb(10));
    double action_prob =
        test.get_action_probability(action, PhaseType::Preflop, 2);
    CHECK_CLOSE(0.0, action_prob, 0.01);
  }
}
