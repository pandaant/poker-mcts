#include <UnitTest++.h>
#include <fcontext.hpp>
#include <fplayer.hpp>
#include <fseat.hpp>
#include <fcontext_config.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/random_handlist.hpp>
#include <iostream>

SUITE(FreedomContextTests) {
  using namespace poker;
  using namespace Freedom5;

  struct Setup {
    Handlist *bot_hl;
    vector<FSeat> players;
    FContextConfig *cconfig;
    FContext context;
    vector<Action> actions, expected;

    Setup()
        : players({FSeat(FPlayer("mark", bb(10)), StatusType::Active),
                   FSeat(FPlayer("simon", bb(10)), StatusType::Active), }),
          cconfig(new FContextConfig(Hand("AhAs"), 5, vector<unsigned>(),
                                     vector<double>({0.5, 1}),
                                     vector<double>({2, 3}))),
          context(bb(2), bb(0), 0, 0, 0, 0, 0, PhaseType::Preflop,
                           players, Action(ActionType::None, bb(0)), cconfig) {
      bot_hl = new ecalc::SingleHandlist(Hand("AhKh"));
    }

    ~Setup() {
      delete cconfig;
      delete bot_hl;
    }
  };

  struct ComplexSetup {
    FContextConfig *cconfig;
    vector<FSeat> players;
    FContext context;
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

    ComplexSetup()
        : pot(bb(1.5)), highest_bet(bb(1)), index_bot(0), index_utg(0),
          index_button(0), index_active(0), betting_round(0),
          board(vector<unsigned>()), phase(PhaseType::Preflop),
          bot_hl(new ecalc::SingleHandlist(Hand("AhKh"))),
          players({FSeat(FPlayer("mark", bb(10), vector<bb>(4, bb(0))),
                         StatusType::Active),
                   FSeat(FPlayer("simon", bb(10),
                                 vector<bb>({bb(0.5), bb(0), bb(0), bb(0)})),
                         StatusType::Active),
                   FSeat(FPlayer("fish", bb(10),
                                 vector<bb>({bb(1), bb(0), bb(0), bb(0)})),
                         StatusType::Active)}),
          random(new ecalc::RandomHandlist()),
          cconfig(new FContextConfig(Hand("AhKh"), 2, board,
                                     vector<double>({1}), vector<double>({3}))),
          context(pot, highest_bet, index_bot, index_utg,
                               index_button, index_active, betting_round, phase,
                               players, Action(ActionType::None, bb(0)),
                               cconfig) {

      players[0].player.handlist = bot_hl;
      players[1].player.handlist = random;
      players[2].player.handlist = random;
    }

    ~ComplexSetup() {
      delete cconfig;
      delete bot_hl;
      delete random;
    }
  };

  TEST_FIXTURE(Setup, TestCounters) {
    CHECK_EQUAL(2, context.nb_player_active());
    CHECK_EQUAL(0, context.nb_player_inactive());
    CHECK_EQUAL(0, context.nb_player_allin());

    context.seats[0].set_inactive();

    CHECK_EQUAL(1, context.nb_player_active());
    CHECK_EQUAL(1, context.nb_player_inactive());

    context.seats[1].set_allin();

    CHECK_EQUAL(0, context.nb_player_active());
    CHECK_EQUAL(1, context.nb_player_inactive());
    CHECK_EQUAL(1, context.nb_player_allin());
  }

  TEST_FIXTURE(Setup, TestHasBet) {
    CHECK_EQUAL(false, context.has_bet());
    context.highest_bet = 1;
    CHECK_EQUAL(true, context.has_bet());
  }

  TEST_FIXTURE(Setup, TestIsTerminal) {
    CHECK_EQUAL(false, context.is_terminal());
    context.phase = PhaseType::Showdown;
    CHECK_EQUAL(true, context.is_terminal());
    context.phase = PhaseType::Preflop;

    context.seats[0].set_allin();
    context.index_active = -1;
    CHECK_EQUAL(true, context.is_terminal());
    context.seats[0].set_active();
    context.index_active = 0;

    context.seats[0].set_inactive();
    CHECK_EQUAL(true, context.is_terminal());
  }

  TEST_FIXTURE(Setup, TestNextUtg) {
    CHECK_EQUAL(0, context.index_utg);
    context.index_utg = context.next_utg();
    CHECK_EQUAL(1, context.index_utg);
    context.index_button = 1;
    context.index_utg = context.next_utg();
    CHECK_EQUAL(0, context.index_utg);
  }

  TEST_FIXTURE(Setup, TestNextToAct) {
    CHECK_EQUAL(1, context.next_to_act());
    context.index_active = 1;
    CHECK_EQUAL(0, context.next_to_act());

    context.highest_bet = -1;
    CHECK_EQUAL(-1, context.next_to_act());
  }

  TEST_FIXTURE(Setup, TestEnumAvailableActionsXR) {
    vector<ActionType::Enum> ex({ActionType::Check, ActionType::Raise});

    vector<ActionType::Enum> res = context.enum_available_actions();
    for (unsigned i = 0; i < ex.size(); ++i)
      CHECK(ex[i] == res[i]);
  }

  TEST_FIXTURE(Setup, TestAvailableActionsXRR) {
    CHECK_EQUAL(bb(10), context.seats[0].player.bankroll);
    actions = context.available_actions();
    CHECK_EQUAL(3, actions.size());
    expected = vector<Action>({Action(ActionType::Check, bb(0)),
                               Action(ActionType::Raise, bb(1)),
                               Action(ActionType::Raise, bb(2))});

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestEnumAvailableActionsFCR) {
    context.highest_bet = 1;
    vector<ActionType::Enum> ex(
        {ActionType::Fold, ActionType::Call, ActionType::Raise});

    vector<ActionType::Enum> res = context.enum_available_actions();
    for (unsigned i = 0; i < ex.size(); ++i)
      CHECK(ex[i] == res[i]);
  }

  TEST_FIXTURE(Setup, TestAvailableActionsFCRR) {
    context.highest_bet = 1;
    actions = context.available_actions();
    CHECK_EQUAL(4, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(1)),
                               Action(ActionType::Raise, bb(2)),
                               Action(ActionType::Raise, bb(3))});

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
      // std::cout << actions[i].to_str() << " " << actions[i].amount <<
      // std::endl;
    }
  }

  TEST_FIXTURE(Setup, TestEnumAvailableActionsFC) {
    context.highest_bet = bb(100);
    vector<ActionType::Enum> ex({ActionType::Fold, ActionType::Call});

    vector<ActionType::Enum> res = context.enum_available_actions();
    for (unsigned i = 0; i < ex.size(); ++i)
      CHECK(ex[i] == res[i]);
  }

  TEST_FIXTURE(Setup, TestAvailableActionsFC) {
    context.highest_bet = bb(100);
    actions = context.available_actions();
    CHECK_EQUAL(2, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(10)), });

    for (unsigned i = 0; i < actions.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestEnumAvailableActionsFCR2) {
    context.highest_bet = bb(100);
    context.seats[0].player.bankroll = bb(110);
    vector<ActionType::Enum> ex(
        {ActionType::Fold, ActionType::Call, ActionType::Raise});

    vector<ActionType::Enum> res = context.enum_available_actions();
    for (unsigned i = 0; i < ex.size(); ++i)
      CHECK(ex[i] == res[i]);
  }

  TEST_FIXTURE(Setup, TestAvailableActionsFCA) {
    context.highest_bet = bb(100);
    context.seats[0].player.bankroll = bb(110);
    actions = context.available_actions();
    CHECK_EQUAL(3, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(100)),
                               Action(ActionType::Raise, bb(110))});

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestAvailableActionsOpponentFC) {
    context.index_active = 1;
    context.highest_bet = bb(100);
    actions = context.available_actions();
    CHECK_EQUAL(2, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(10)), });

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestAvailableActionsOpponentFCR) {
    context.index_active = 1;
    context.highest_bet = bb(2);
    actions = context.available_actions();
    CHECK_EQUAL(3, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(2)),
                               Action(ActionType::Raise, bb(4))});

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestAvailableActionsOpponentFCA) {
    context.highest_bet = bb(100);
    context.index_active = 1;
    context.seats[1].player.bankroll = bb(110);
    actions = context.available_actions();
    CHECK_EQUAL(3, actions.size());

    expected = vector<Action>({Action(ActionType::Fold, bb(0)),
                               Action(ActionType::Call, bb(100)),
                               Action(ActionType::Raise, bb(110))});

    for (unsigned i = 0; i < expected.size(); ++i) {
      CHECK(expected[i] == actions[i]);
    }
  }

  TEST_FIXTURE(Setup, TestTransitionPhaseBasic) {
    CHECK_EQUAL(PhaseType::Preflop, context.phase);
    context.transition_phase();
    CHECK_EQUAL(PhaseType::Flop, context.phase);
    context.transition_phase();
    CHECK_EQUAL(PhaseType::Turn, context.phase);
    context.transition_phase();
    CHECK_EQUAL(PhaseType::River, context.phase);
    context.transition_phase();
    CHECK_EQUAL(PhaseType::Showdown, context.phase);
  }

  TEST_FIXTURE(Setup, TestTransitionPhaseNbPlayer) {
    context.seats[0].set_inactive();
    context.transition_phase();
    CHECK_EQUAL(PhaseType::Showdown, context.phase);
  }

  TEST_FIXTURE(Setup, TestLastToAct) {
    CHECK_EQUAL(false, context.is_last_to_act());
    context.index_active = 1;
    // because index 0 is index_utg
    CHECK_EQUAL(true, context.is_last_to_act());
  }

  // to check if we handle floatingpoint operations correctly
  TEST_FIXTURE(ComplexSetup, TestFloatingPointOperationsEnumAvailableActions) {
    context.highest_bet = bb(96.0);
    context.seats[0].player.bankroll = bb(33.000000000000007);
    context.seats[0].player.invested[0] = bb(63.0);
    vector<poker::ActionType::Enum> actions = context.enum_available_actions();
    CHECK_EQUAL(2, actions.size());
    CHECK_EQUAL(poker::ActionType::Fold, actions[0]);
    CHECK_EQUAL(poker::ActionType::Call, actions[1]);
  }

  TEST_FIXTURE(Setup, TestTransitionAction) {
    actions = context.available_actions();

    FContext tcheck = context.transition(actions[0]);
    FContext traise_small = context.transition(actions[1]);
    FContext traise_big = context.transition(actions[2]);

    CHECK_EQUAL(PhaseType::Preflop, tcheck.phase);
    CHECK_EQUAL(1, tcheck.index_active);

    CHECK_EQUAL(bb(3), traise_small.pot);
    CHECK_EQUAL(bb(1), traise_small.highest_bet);
    CHECK_EQUAL(bb(9), traise_small.seats[0].player.bankroll);
    CHECK_EQUAL(1, traise_small.betting_round);

    CHECK_EQUAL(bb(4), traise_big.pot);
    CHECK_EQUAL(bb(2), traise_big.highest_bet);
    CHECK_EQUAL(bb(8), traise_big.seats[0].player.bankroll);
    CHECK_EQUAL(1, traise_big.betting_round);

    actions = traise_small.available_actions();
    FContext ttfold = traise_small.transition(actions[0]);
    FContext ttcall = traise_small.transition(actions[1]);
    FContext ttraise = traise_small.transition(actions[2]);

    // when opponent folds we win
    CHECK_EQUAL(true, ttfold.seats[1].is_inactive());
    CHECK_EQUAL(true, ttfold.is_terminal());
    CHECK_EQUAL(PhaseType::Showdown, ttfold.phase);

    CHECK_EQUAL(bb(4), ttcall.pot);
    CHECK_EQUAL(bb(0), ttcall.highest_bet);
    CHECK_EQUAL(0, ttcall.betting_round);
    CHECK_EQUAL(PhaseType::Flop, ttcall.phase);
    CHECK_EQUAL(bb(9), ttcall.seats[1].player.bankroll);

    // we raise, and stay in preflop
    CHECK_EQUAL(bb(5), ttraise.pot);
    CHECK_EQUAL(bb(2), ttraise.highest_bet);
    CHECK_EQUAL(2, ttraise.betting_round);
    CHECK_EQUAL(PhaseType::Preflop, ttraise.phase);
    CHECK_EQUAL(bb(8), ttraise.seats[1].player.bankroll);
  }

  TEST_FIXTURE(ComplexSetup, TestTransitionMaxBettingRounds) {
    FContext bt1 =
        context.transition(Action(ActionType::Raise, bb(2)));
    CHECK_EQUAL(1, bt1.betting_round);
    FContext bt2 =
        bt1.transition(Action(ActionType::Raise, bb(4)));
    CHECK_EQUAL(2, bt2.betting_round);

    CHECK_EQUAL(2, bt2.index_active);
    actions = bt2.available_actions();
    for (auto action : actions)
      CHECK(action.action != ActionType::Raise &&
            action.action != ActionType::AllIn);
  }

  TEST_FIXTURE(ComplexSetup, TestComplexSetup) {
    // transition bot - call , simon call, fish check
    FContext b1 =
        context.transition(Action(ActionType::Call, bb(1)));
    CHECK_EQUAL(1, b1.index_active);
    FContext b2 =
        b1.transition(Action(ActionType::Call, bb(0.5)));
    CHECK_EQUAL(2, b2.index_active);
    FContext b3 =
        b2.transition(Action(ActionType::Check, bb(0)));

    CHECK_EQUAL(PhaseType::Flop, b3.phase);
    CHECK_EQUAL(bb(3), b3.pot);
    CHECK_EQUAL(bb(0), b3.highest_bet);
    CHECK_EQUAL(1, b3.index_active);
    CHECK_EQUAL(1, b3.index_utg);

    FContext b4 =
        b3.transition(Action(ActionType::Raise, bb(1.5)));
    CHECK_EQUAL(2, b4.index_active);
    FContext b5 =
        b4.transition(Action(ActionType::Fold, bb(0)));
    CHECK_EQUAL(0, b5.index_active);
    FContext b6 =
        b5.transition(Action(ActionType::Call, bb(1.5)));

    CHECK_EQUAL(PhaseType::Turn, b6.phase);
    CHECK_EQUAL(bb(6), b6.pot);
    CHECK_EQUAL(bb(0), b6.highest_bet);
    CHECK_EQUAL(1, b6.index_active);
    CHECK_EQUAL(1, b6.index_utg);
  }

  TEST_FIXTURE(ComplexSetup, TestTransitionRaiseBiggerBankroll) {

    FContext b1 =
        context.transition(Action(ActionType::Raise, bb(20)));
    CHECK_EQUAL(1, b1.index_active);
    CHECK_EQUAL(bb(0), b1.seats[0].player.bankroll);
    CHECK_EQUAL(bb(10), b1.seats[0].player.invested[phase]);
    CHECK_EQUAL(bb(11.5), b1.pot);
    CHECK_EQUAL(bb(10), b1.highest_bet);
    CHECK_EQUAL(poker::StatusType::Allin, b1.seats[0].status);
  }

  TEST_FIXTURE(ComplexSetup, TestTransitionCallBiggerBankroll) {
    context.seats[1].player.bankroll = bb(5);
    FContext b1 =
        context.transition(Action(ActionType::Raise, bb(20)));
    CHECK_EQUAL(1, b1.index_active);
    FContext b2 =
        b1.transition(Action(ActionType::Call, bb(10)));
    CHECK_EQUAL(2, b2.index_active);

    CHECK_EQUAL(bb(0), b2.seats[1].player.bankroll);
    CHECK_EQUAL(bb(5.5), b2.seats[1].player.invested[phase]);
    CHECK_EQUAL(bb(16.5), b2.pot);
    CHECK_EQUAL(bb(10), b2.highest_bet);
    CHECK_EQUAL(poker::StatusType::Allin, b2.seats[1].status);
  }

  TEST_FIXTURE(ComplexSetup, TestTransitionFCR1) {
    players =
        vector<FSeat>({FSeat(FPlayer("mark", bb(10),
                                     vector<bb>({bb(1), bb(0),
                                                     bb(0), bb(0)})),
                             StatusType::Active),
                       FSeat(FPlayer("simon", bb(10),
                                     vector<bb>({bb(1.5), bb(0),
                                                     bb(0), bb(0)})),
                             StatusType::Active)});

    context =
        FContext(bb(2.5), bb(1.5), index_bot, index_utg,
                     index_button, index_active, betting_round, phase, players,
                     Action(ActionType::None, bb(0)), cconfig);

    vector<Action> actions = context.available_actions();
    CHECK_EQUAL(ActionType::Fold, actions[0].action);
    CHECK_EQUAL(ActionType::Call, actions[1].action);
    CHECK_EQUAL(ActionType::Raise, actions[2].action);

    vector<FContext> transitions = context.transition();

    CHECK_EQUAL(3, transitions.size());
    FContext c1 = transitions[0];
    FContext c2 = transitions[1];
    FContext c3 = transitions[2];

    CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
    CHECK_EQUAL(ActionType::Call, c2.last_action.action);
    CHECK_EQUAL(ActionType::Raise, c3.last_action.action);
  }

  // BugTest
  // Problem: if the active player goes allin, the index
  // utg is set to this player which results in an infinite loop
  // because next_to_act() will not return the utg index ever ( because he is
  // allin )
  TEST_FIXTURE(ComplexSetup, TestNextUtgActiveIndexGoesAllin) {
    FContext c1 = context.transition(poker::Action(ActionType::AllIn, bb(10)));
    CHECK_EQUAL(1, c1.index_active);
    CHECK_EQUAL(1, c1.index_utg);
  }

  TEST_FIXTURE(ComplexSetup, TestNextUtgActiveIndexGoesAllin2) {
    players =
        vector<FSeat>({FSeat(FPlayer("mark", bb(10),
                                     vector<bb>({bb(0), bb(0),
                                                     bb(0), bb(0)})),
                             StatusType::Active),
                       FSeat(FPlayer("simon", bb(0),
                                     vector<bb>({bb(10), bb(0),
                                                     bb(0), bb(0)})),
                             StatusType::Allin)});

    context = FContext(bb(11), bb(10), index_bot, 1, index_button,
                           index_active, betting_round, phase, players,
                           Action(ActionType::None, bb(0)), cconfig);

    FContext c1 =
        context.transition(poker::Action(ActionType::Call, bb(10)));
    CHECK_EQUAL(-1, c1.index_active);
    CHECK_EQUAL(-1, c1.index_utg);
  }

   //BugTest
   //when a player goes allin the player after him is the new utg.
   //but when this player then folds, the utg does not get updated,
   //which results in an endless loop.
  TEST_FIXTURE(ComplexSetup, TestNewUtgWhenUtgPlayerFolds) {
    FContext c1 =
        context.transition(poker::Action(ActionType::AllIn, bb(10)));

    CHECK_EQUAL(1, c1.index_utg);
    CHECK_EQUAL(1, c1.index_active);

    FContext c2 =
        c1.transition(poker::Action(ActionType::Fold, bb(0)));

    CHECK_EQUAL(2, c2.index_utg);
    CHECK_EQUAL(2, c2.index_active);
    CHECK_EQUAL(false, c2.is_terminal());
  }

  TEST_FIXTURE(ComplexSetup, TestNewUtgWhenUtgPlayerFolds2) {
    context.index_utg = 1;
    context.index_active = 1;

    FContext c1 =
        context.transition(poker::Action(ActionType::AllIn, bb(10)));

    CHECK_EQUAL(2, c1.index_utg);
    CHECK_EQUAL(2, c1.index_active);

    FContext c2 =
        c1.transition(poker::Action(ActionType::Fold, bb(0)));

    CHECK_EQUAL(0, c2.index_utg);
    CHECK_EQUAL(0, c2.index_active);
    CHECK_EQUAL(false, c2.is_terminal());
  }

   //BugTest
   //when a player goes allin by calling and is the index_utg
   //the next_utg remains unchanged which causes bad bad inifinite
   //calculations in the mcts
  TEST_FIXTURE(ComplexSetup, TestNewUtgWhenUtgPlayerCallsAllin) {
    context.seats[1].player.bankroll = bb(9.5);
    FContext c1 = 
        context.transition(poker::Action(ActionType::AllIn, bb(10)));

    CHECK_EQUAL(1, c1.index_utg);
    CHECK_EQUAL(1, c1.index_active);

    FContext c2 =
        c1.transition(poker::Action(ActionType::Call, bb(9.5)));

    CHECK_EQUAL(2, c2.index_utg);
    CHECK_EQUAL(2, c2.index_active);
    CHECK_EQUAL(false, c2.is_terminal());
  }
}

