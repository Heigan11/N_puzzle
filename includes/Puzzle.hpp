/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Puzzle.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchelste <bchelste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:19:42 by bchelste          #+#    #+#             */
/*   Updated: 2022/10/03 15:22:52 by bchelste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <queue>
#include <cmath>
#include <iostream>
#include <fstream>

#include "PuzzleData.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "State.hpp"

class Solver;

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
		
	private:

		std::string				heuristic;
		std::string				search;
		PuzzleData				puzzle;
		Parser					parser;
		Solver					*solver;

		void					printSolution(std::string sState, std::string heuristic);

		void					clearStates();

		void					solve();

		void					manhattanHeuristic(State *state);
		void					euclidianHeuristic(State *state);
		void					hammingHeuristic(State *state);
		
};