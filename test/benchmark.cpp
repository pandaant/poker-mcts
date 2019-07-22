#include <chrono>
#include <string>
#include <UnitTest++/UnitTest++.h>
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

  Handranks handranks("../handranks.dat");

  void print_benchmark_result(string name, system_clock::duration duration,
                              unsigned iterations, unsigned nb_nodes,
                              size_t tree_s) {
    std::cout << name << " | Iterations: " << iterations
              << " | nbNodes: " << nb_nodes
              << " | TreeSize: " << tree_s / 1024.0 << " MB"
              << " | Time: " << duration_cast<milliseconds>(duration).count()
              << " ms" << std::endl;
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 3 players, preflop. bot is under the
  ///          gun with AsAh. sampling for 1 second. 7 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark3pNodesFromPreflop) {
    unsigned samples = 7;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{FPlayer(bb(10)),
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 0, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("3pPfUTG7s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 6 players, preflop. bot is under the
  ///          gun with AsAh. sampling for 1 second. 7 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark6pNodesFromPreflop) {
    unsigned samples = 7;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
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

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("6pPfUTG7s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 9 players, preflop. bot is under the
  ///          gun with AsAh. sampling for 1 second. 7 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark9pNodesFromPreflop) {
    unsigned samples = 7;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
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
                           FPlayer(bb(10)),
                           FPlayer(bb(10)),
                           FPlayer(bb(10)),
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 6, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("9pPfUTG7s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 4 players, flop. bot is under the
  ///          gun. sampling for 1 second. 16 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark4pNodesFromFlop) {
    unsigned samples = 16;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{FPlayer(bb(10)), FPlayer(bb(10)),
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

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("4pPUTG16s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 4 players, flop. bot is under the
  ///          gun. sampling for 1 second. 64 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark4pNodesFromFlop64Samples) {
    unsigned samples = 64;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{FPlayer(bb(10)), FPlayer(bb(10)),
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

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("4pPUTG64s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 3 players, flop. bot is under the
  ///          gun. sampling for 1 second. 64 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark3pNodesFromFlop64Samples) {
    unsigned samples = 64;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{FPlayer(bb(10)),
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 2, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("3pPUTG64s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 2 players, turn. bot is under the
  ///          gun. sampling for 1 second. 16 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark2pNodesFromTurn16Samples) {
    unsigned samples = 16;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 1, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("2pTUTGr16s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }

  // ----------------------------------------------------------------------
  /// @brief   benchmarks freedom with 2 players, turn. bot is under the
  ///          gun. sampling for 1 second. 64 samples for showdown
  ///          eval.
  ///
  /// @param BenchmarkNodesFromPreflop
  // ----------------------------------------------------------------------
  TEST(Benchmark2pNodesFromTurn64Samples) {
    unsigned samples = 64;
    ECalc *calc = new ECalc(&handranks);
    FConfig *config = new FConfig(
        1.0, samples, calc, new ShowdownEval(calc, samples),
        new AvgBackpropagationStrategy(), new AvgBackpropagationStrategy(),
        new SamplingSelector<FContext, FConfig>(),
        new SamplingSelector<FContext, FConfig>(),
        new MaxValueSelector<FContext, FConfig>());

    FContextConfig *fconfig =
        new FContextConfig(Hand("AhAs"), 5, {}, {1.0}, {0});

    RandomHandlist *random = new RandomHandlist();

    vector<FPlayer> player{
                           FPlayer(bb(9.5), {bb(0.5), bb(0), bb(0), bb(0)}),
                           FPlayer(bb(9), {bb(1), bb(0), bb(0), bb(0)})};

    for (unsigned i = 0; i < player.size(); ++i) {
      player[i].handlist = random;
    }

    FContext *context = new FContext(0, 0, 1, 0, player, fconfig);

    std::chrono::seconds runtime(1);
    MCTS<FContext, FConfig, DecisionNode, Response> mcts(*context, config);

    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start < runtime)
      mcts.iterate();

    unsigned nb_nodes = mcts.nb_nodes();
    print_benchmark_result("2pTUTG64s", runtime, mcts.nb_iterations(), nb_nodes,
                           sizeof(*mcts.root()) * nb_nodes);
  }
}

