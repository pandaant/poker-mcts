# NLTH Poker Agent using Monte Carlo Simulation

Monte Carlo Tree Search or short MCTS is a simulation based search algorithm that can be
used to solve extensive-form games with imperfect information. Complex games like NLTH
have a large number of states due to the element of chance and the number of opponents.
Instead of searching the complete state space (which is intractable) MCTS only samples one
possible outcome per iteration. As the number of iterations increases, the true value of a state
can be approximated.
MCTS is a anytime algorithm which means that the computation can be interrupted at any time
to retrieve a valid result (or a approximation of it if not enough iterations have been performed.)
The search is performed on a tree containing the following types of nodes:

**Root node** is the start of the tree. The node represents the state we want to evaluate.

**Action nodes** are nodes in which the same player has to make a decision as in the root node.

**Opponent nodes** represent the opposing players decisions.

**Terminal nodes** represent either nodes in which the game ended by folding by all but one player or by going to a showdown.

**Chance nodes** are used in places where chance makes a move (draw a card).

Each iteration begins at the root node and recursively selects children according to a selection
strategy that chooses which action to take. If the state for the selected action does not exist,
the last state is expanded and it’s children are added to the tree before continuing the selection
until a terminal history is reached. This history is then evaluated according to a simulation
strategy that calculates the expected value. In the final step of the iteration the calculated value
is propagated back up the tree by an backpropagation strategy. Figure 3.13 shows a graphical representation of one iteration. After a certain amount of time or number of predefined
iterations a final move strategy selects the actual actions to play from the root node.

## Requirements

* Clang (For C++11 support)
* UnitTest++ for tests (expects headers and lib to be in /usr/{lib,include})

## Installation
* Clone repository:
```bash
$ git clone --recursive https://github.com/pandaant/poker-mcts
```
* Download [HandRanks.dat](https://github.com/christophschmalhofer/poker/blob/master/XPokerEval/XPokerEval.TwoPlusTwo/HandRanks.dat) and save it as poker-mcts/handranks.dat
* Build agent library:
```shell
$ cd poker-mcts
$ make all
```
* build the documentation:
```shell
$ make doc
```
A compiled version of the doxygen documentation can be found [here](http://mark-zumbruch.de/documentation/poker-mcts/)
* run the tests:
```shell
$ cd test
$ make all run
```


## Usage