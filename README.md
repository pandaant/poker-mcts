# NLTH Poker Agent using Monte Carlo Simulation

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