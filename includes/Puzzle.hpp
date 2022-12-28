
#pragma once

#include <queue>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <regex>

#include "PuzzleData.hpp"
#include "Parser.hpp"
#include "Runner.hpp"
#include "State.hpp"

class Runner;

class Puzzle
{

	public:

		Puzzle();
		Puzzle(PuzzleData &newPuzzle, std::string &heuristicType, std::string &searchType);
		virtual~Puzzle();

		std::string				getHeuristic();
		std::string				getSearch();
		PuzzleData				getPuzzle();

		void					parseData(int argc, char **argv);
		void					solvePuzzle();

		void					printState(std::map<int, int> &state);

		void 					printStats();
		void 					printCompare();

	private:

		std::string				heuristic;
		std::string				search;
		PuzzleData				puzzle;
		Parser					parser;
		Runner					*solver;

		void					printSolution(std::string sState, std::string heuristic, uint64_t startTime);

		void					clearStates();

		void					solve();

		void					manhattanHeuristic(State *state);
		void					euclidianHeuristic(State *state);
		void					hammingHeuristic(State *state);

};