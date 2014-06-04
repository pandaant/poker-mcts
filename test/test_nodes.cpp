#include <UnitTest++.h>
#include <fcontext.hpp>
#include <fcontext_config.hpp>
#include <fconfig.hpp>
#include <fplayer.hpp>
#include <fseat.hpp>
#include <fcontext_config.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/random_handlist.hpp>
#include <decision_node.hpp>
#include <mcts/max_value_selector.hpp>
#include <mcts/avg_backpropagation_strategy.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <showdown_eval.hpp>
#include <showdown_node.hpp>
#include <opponent_node.hpp>
#include <const_leaf_node.hpp>
#include <iostream>
#include <string.h>
#include <mcts/root_node.hpp>
#include <ecalc/handranks.hpp>

SUITE(FreedomNodeTests) {
  using namespace freedom;
  using namespace mcts;
  using namespace poker;

  typedef typename ISelectionStrategy<FContext, FConfig>::sstrategy_t sstrategy_t;

  ecalc::Handranks handranks("../../../bin/data/handranks.dat");

  struct ComplexSetup {
    FConfig *config;
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
    double rake_factor;

    ECalc *calc;
    int max_betting_rounds;
    double time_s;
    int ecalc_nb_samples;
    sstrategy_t *decision_selection_strat;
    sstrategy_t *opponent_selection_strat;
    IBackpropagationStrategy *decision_backprop_strat;
    IBackpropagationStrategy *opponent_backprop_strat;
    ISimulationStrategy<FContext> *sim_strat;

    ComplexSetup()
        : players({FSeat(FPlayer("mark", bb(10), vector<bb>({ bb(5), bb(0), bb(0), bb(0) })),
                          StatusType::Active),
                    FSeat(FPlayer("simon", bb(10), vector<bb>({ bb(5), bb(0), bb(0), bb(0) })),StatusType::Active) }),
          random(new ecalc::RandomHandlist()) {
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
      bot_hl = new ecalc::SingleHandlist(Hand("AhAs"));

      players[0].player.handlist = bot_hl;
      players[1].player.handlist = random;
      // players[2].player.handlist = random;

      cconfig = new FContextConfig(Hand("AhAs"), 2, board, vector<double>({1}),
                                   vector<double>({3}), rake_factor);
      context =
          new FContext(pot, highest_bet, index_bot, index_utg, index_button,
                       index_active, betting_round, phase, players,
                       Action(ActionType::None, bb(0)), cconfig);

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
    }

    ~ComplexSetup() {
      delete context;
      delete cconfig;
      delete bot_hl;
      delete random;

      delete decision_selection_strat;
      delete opponent_selection_strat;
      delete decision_backprop_strat;
      delete opponent_backprop_strat;
      delete config;
    }
  };

   TEST_FIXTURE(ComplexSetup, TestDecisionNodeExpandXR) {
   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(10), vector<bb>({ bb(1), bb(0),
  bb(0), bb(0) })),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(10), vector<bb>({ bb(0.5), bb(0),
   bb(0), bb(0) })),
   StatusType::Active) });

   context = new FContext(bb(1.5), bb(1), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   DecisionNode *dn = new DecisionNode(context->clone(), config, NULL);
   dn->expand();
   CHECK_EQUAL(2, dn->children().size());

   FContext c1 =
   dn->children()[0]->context();
   FContext c2 =
   dn->children()[1]->context();
   CHECK_EQUAL(ActionType::Check, c1.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c2.last_action.action);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestExpandNotPossible){
  //TODO
  }

   //test multiple raise sizes
 
   TEST_FIXTURE(ComplexSetup, TestDecisionNodeExpandXRRR) {
   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(10), vector<bb>({ bb(1), bb(0),
  bb(0), bb(0) })),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(10), vector<bb>({ bb(0.5), bb(0),
   bb(0), bb(0) })),
   StatusType::Active) });

   cconfig = new FContextConfig(Hand("AhAs"), 2, board, vector<double>({ 1 }),
   vector<double>({ 3, 4, 5 }), rake_factor);
   context = new FContext(bb(1.5), bb(1), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   DecisionNode *dn = new DecisionNode(context->clone(), config, NULL);
   dn->expand();
   CHECK_EQUAL(4, dn->children().size());

   FContext c1 = dn->children()[0]->context();
   FContext c2 = dn->children()[1]->context();
   FContext c3 = dn->children()[2]->context();
   FContext c4 = dn->children()[3]->context();

   CHECK_EQUAL(ActionType::Check, c1.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c4.last_action.action);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestDecisionNodeExpandFCR) {

   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(10), vector<bb>({ bb(1), bb(0),
  bb(0), bb(0) })),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(10), vector<bb>({ bb(1.5), bb(0),
   bb(0), bb(0) })),
   StatusType::Active) });

   context = new FContext(bb(2.5), bb(1.5), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   DecisionNode *dn = new DecisionNode(context->clone(), config, NULL);
   dn->expand();

   CHECK_EQUAL(3, dn->children().size());

   FContext c1 = dn->children()[0]->context();
   FContext c2 = dn->children()[1]->context();
   FContext c3 = dn->children()[2]->context();

   CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
   CHECK_EQUAL(true, c1.is_terminal());

   CHECK_EQUAL(ActionType::Call, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestDecisionNodeExpandAllIn) {

   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(9), vector<bb>({ bb(1), bb(0),
  bb(0), bb(0) })),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(2), vector<bb>({ bb(8), bb(0),
   bb(0), bb(0) })),
   StatusType::Active) });

   context = new FContext(bb(9), bb(8), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   DecisionNode *dn = new DecisionNode(context->clone(), config, NULL);
   dn->expand();

   CHECK_EQUAL(3, dn->children().size());

   FContext c1 = dn->children()[0]->context();
   FContext c2 = dn->children()[1]->context();
   FContext c3 = dn->children()[2]->context();

   CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
   CHECK_EQUAL(true, c1.is_terminal());

   CHECK_EQUAL(ActionType::Call, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestDecisionNodeExpandToShowdown) {

   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(5), vector<bb>({ bb(5), bb(0),
  bb(0), bb(0) }), random),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(2), vector<bb>({ bb(5), bb(0),
   bb(0), bb(3) }), random),
   StatusType::Active) });

   phase = PhaseType::River;
   betting_round = 1;
   index_utg = 1;
   context = new FContext(bb(13), bb(3), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   DecisionNode *dn = new DecisionNode(context->clone(), config, NULL);
   dn->expand();

   CHECK_EQUAL(3, dn->children().size());

   FContext c1 = dn->children()[0]->context();
   FContext c2 = dn->children()[1]->context();
   FContext c3 = dn->children()[2]->context();

   CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
   CHECK_EQUAL(true, c1.is_terminal());

   CHECK_EQUAL(ActionType::Call, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);

  // c2 should be showdown node
   CHECK_EQUAL(true, c2.is_terminal());

   vector<Action> actions = c2.available_actions();
   CHECK_EQUAL(0, actions.size());

   try {
   dn->children()[1]->simulate();
   CHECK(true);
  }
   catch (std::exception e) {
   CHECK(false);
  }

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestOpponentNodeExpandToShowdown) {

   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(2), vector<bb>({ bb(5), bb(0),
  bb(0), bb(3) }), random),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(5), vector<bb>({ bb(5), bb(0),
   bb(0), bb(0) }), random),
   StatusType::Active) });

   phase = PhaseType::River;
   index_utg = 0;
   index_active = 1;
   betting_round = 1;
   players[0].player.handlist = random;
   players[1].player.handlist = random;
   context = new FContext(bb(13), bb(3), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   RootNode<FContext, FConfig, DecisionNode> *root = new RootNode<FContext, FConfig, DecisionNode>(*context, config);
   OpponentNode *dn = new OpponentNode(context->clone(), config, root);
   dn->expand();

   FContext cc = dn->context();
   cc.transition(Action(ActionType::Call, bb(3)));
  //    delete cc;

   vector<Action> actions = context->available_actions();
   CHECK_EQUAL(3, dn->children().size());
  // for(auto ac : actions) std::cout << ActionType::ToStr[ac.action] << " "
  // << ac.amount << std::endl;

   FContext c1 = dn->children()[0]->context();
   FContext c2 = dn->children()[1]->context();
   FContext c3 = dn->children()[2]->context();

   CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
   CHECK_EQUAL(true, c1.is_terminal());

   CHECK_EQUAL(ActionType::Call, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);

  // c2 should be showdown node
   CHECK_EQUAL(true, c2.is_terminal());

   actions = c2.available_actions();
   CHECK_EQUAL(0, actions.size());

   for (int i = 0; i < 1000; ++i)
   dn->children()[1]->backpropagate(dn->children()[1]->simulate());
   CHECK_CLOSE(0, dn->children()[1]->ev(), 0.1);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestOpponentNodeExpandFCR) {

   players = vector<FSeat>(
  { FSeat(FPlayer("mark", bb(10), vector<bb>({ bb(1.5), bb(0),
  bb(0), bb(0) })),
   StatusType::Active),
   FSeat(FPlayer("simon", bb(10), vector<bb>({ bb(1), bb(0),
   bb(0), bb(0) })),
   StatusType::Active) });

   index_utg = 1;
   index_active = 1;
   context = new FContext(bb(2.5), bb(1.5), index_bot, index_utg,
   index_button,
   index_active, betting_round, phase, players,
   Action(ActionType::None, bb(0)), cconfig);

   CHECK_EQUAL(bb(1.5), players[0].player.total_investment());
   CHECK_EQUAL(bb(1.5), players[0].player.invested[PhaseType::Preflop]);
   CHECK_EQUAL(bb(1.5), context->seats[0].player.total_investment());
   CHECK_EQUAL(bb(1.5),
   context->seats[0].player.invested[PhaseType::Preflop]);

   OpponentNode *dn = new OpponentNode(context->clone(), config, NULL);
   dn->expand();

   CHECK_EQUAL(3, dn->children().size());

   FContext c1 =
   dn->children()[0]->context();
   FContext c2 =
   dn->children()[1]->context();
   FContext c3 =
   dn->children()[2]->context();

   CHECK_EQUAL(0, c1.index_active);
   CHECK_EQUAL(1, c1.nb_player_active());
   CHECK_EQUAL(PhaseType::Preflop, c1.phase);
   CHECK_EQUAL("mark", c1.get_last_active_seat().player.name);
   CHECK_EQUAL(bb(10), c1.get_last_active_seat().player.bankroll);
   CHECK_EQUAL(bb(1.5),
   c1.get_last_active_seat().player.total_investment());
   CHECK_EQUAL(bb(1.5),
   c1.get_last_active_seat().player.invested[PhaseType::Preflop]);
   CHECK_EQUAL(ActionType::Fold, c1.last_action.action);
   CHECK_EQUAL(true, c1.is_terminal());
   CHECK_CLOSE(1, dn->children()[0]->ev(), 0.1);

   CHECK_EQUAL(ActionType::Call, c2.last_action.action);
   CHECK_EQUAL(ActionType::Raise, c3.last_action.action);

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestConstLeafNode) {
   ConstantLeafNode *dn =
   new ConstantLeafNode(context->clone(), config, NULL, 10);

   CHECK_EQUAL(10, dn->ev());
   CHECK_EQUAL(0, dn->nb_samples());

   delete dn;
  }

   TEST_FIXTURE(ComplexSetup, TestShowdownNode) {
   RootNode<FContext, FConfig, DecisionNode> *root = new RootNode<FContext, FConfig, DecisionNode>(*context, config);
   ShowdownNode *dn = new ShowdownNode(context->clone(), config, root);

   for (int i = 0; i < 1000; ++i)
   dn->backpropagate(dn->simulate());

   CHECK_CLOSE(3.5, dn->ev(), 0.1);

   delete dn;
  }
}
