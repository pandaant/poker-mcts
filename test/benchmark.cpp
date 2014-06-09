#include <chrono>
#include <string>
#include <UnitTest++.h>
#include <mcts/mcts.hpp>
#include <mcts/sampling_selector.hpp>
#include <mcts/avg_backpropagation_strategy.hpp>
#include <mcts/max_value_selector.hpp>
#include <ecalc/handranks.hpp>
#include <ecalc/random_handlist.hpp>
#include <poker/hand.hpp>
#include <poker/pokerdefs.hpp>
#include "fconfig.hpp"
#include "fcontext.hpp"
#include "fcontext_config.hpp"
#include "decision_node.hpp"
#include <showdown_eval.hpp>

#define NB_SAMPLES 7 

SUITE(ECalcBenchmarks) {

  using std::string;
  using std::cout;
  using ecalc::ECalc;
  using ecalc::Handranks;
  using ecalc::RandomHandlist;
  using poker::Hand;
  using poker::bb;
  using freedom::FPlayer;
  using freedom::FConfig;
  using freedom::FContext;
  using freedom::FContextConfig;
  using freedom::ShowdownEval;
  using freedom::DecisionNode;
  using namespace mcts;
  using namespace std::chrono;

  struct Response {};

  Handranks handranks("../../../bin/data/handranks.dat");

  void print_benchmark_result(string name, system_clock::duration duration,
                              unsigned iterations, unsigned nb_nodes, size_t tree_s) {
    std::cout << name << " | Iterations: " << iterations 
              << " | nbNodes: " << nb_nodes 
              << " | TreeSize(MB): " << tree_s / 1024.0 
              << " | Time: " << duration_cast<milliseconds>(duration).count()
              << " ms | " << std::endl;
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 6 players, preflop. bot is under the
  ///          gun with AsAh. sampling for 1 second.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(BenchmarkNodesFromPreflop) {
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(1.0, 10, calc, new ShowdownEval(calc, NB_SAMPLES),
                                  new AvgBackpropagationStrategy(),
                                  new AvgBackpropagationStrategy(),
                                  new SamplingSelector<FContext, FConfig>(),
                                  new SamplingSelector<FContext, FConfig>(),
                                  new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{FPlayer(bb(10)),
                           FPlayer(bb(10)),
                           FPlayer(bb(10)),
                           FPlayer(bb(10)),
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 3, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();

    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    print_benchmark_result("6pPfUTGAA",runtime, mcts.nb_iterations(), mcts.nb_nodes(), sizeof(*mcts.root())*mcts.nb_nodes());
  }
}

