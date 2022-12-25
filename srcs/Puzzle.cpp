/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Puzzle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchelste <bchelste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:19:47 by bchelste          #+#    #+#             */
/*   Updated: 2022/10/03 16:05:47 by bchelste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Puzzle.hpp"

Puzzle::Puzzle()
{
	this->heuristic = "";
	this->search = "";
	this->puzzle = PuzzleData();
	this->parser = Parser();
	this->solver = new Solver(this);
}

Puzzle::Puzzle(PuzzleData &newPuzzle, std::string &heuristicType, std::string &searchType) : 
	heuristic(heuristicType), search(searchType), puzzle(newPuzzle)
{
	this->parser = Parser();
}

Puzzle::~Puzzle()
{
	delete this->solver;
}

std::string	Puzzle::getHeuristic()
{
	return (this->heuristic);
}

std::string Puzzle::getSearch()
{
	return (this->search);
}

PuzzleData Puzzle::getPuzzle()
{
	return (this->puzzle);	
}

void	Puzzle::parseData(int argc, char **argv)
{
	PuzzlePack tmp;
	tmp = this->parser.cmdParser(argc, argv);
	
	this->heuristic = tmp.currentHeuristic;
	this->search = tmp.currentSearch;
	this->puzzle = tmp.puzzleData;
	
}

void Puzzle::solvePuzzle()
{
	solver->setInitialState(this->puzzle.getTiles(), this->puzzle.getPuzzleSize());
	
	// std::cout << "initialState" << std::endl;
	// printState(solver->initialState);
	
	solver->setGoalState();
	
	// std::cout << "-------" << std::endl;
	// std::cout << "goalState" << std::endl;
	// printState(solver->goalState);

	// std::cout << solver->countInversion() << std::endl;
	
	solve();
}

void Puzzle::clearStates()
{
	
	while (solver->opened.size())
	{
		delete solver->opened.top();
		solver->opened.pop();
	}

	while (solver->closed.size())
	{
		delete (*(solver->closed.begin()));
		solver->closed.erase(solver->closed.begin());
	}	
}

void Puzzle::solve()
{
	uint64_t startTime;

	if (solver->isSolvable() == false)
	{
		printSolution("unsolvable", "-", 0);
		clearStates();
		return ;
	}
	
	// printSolution("solvable***");
	// return ;
	
	void (Puzzle::*heuristicFunc)(State *state);
	if (heuristic == "manhtn")
		heuristicFunc = &Puzzle::manhattanHeuristic;
	else if (heuristic == "euclid")
		heuristicFunc = &Puzzle::euclidianHeuristic;
	else
		heuristicFunc = &Puzzle::hammingHeuristic;	
	solver->findSolution(heuristicFunc, search);
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	printSolution("solvable", heuristic, startTime);
	clearStates();
}

void Puzzle::printState(std::map<int, int> &state)
{
	int size = solver->nSize;
	int cntr = 0;
	int i = 0;
	int tmp;

	std::map<int, int>::iterator it;
	
	for (; i < (size * size); ++i)
	{
		// tmp = state.at(i);
		
		it = state.begin();
		for (; it != state.end(); ++it)
		{
			if (i == it->second)
				tmp = it->first;
		}
		
		std::cout << tmp;
		cntr += 1;
		if (cntr == size)
		{
			if (i == ((size * size) - 1))
				break ;
			std::cout << std::endl;
			cntr = 0;
		}
		else
		{
			if (tmp < 10)
				std::cout << "   ";
			else if ((tmp > 9) &&  (tmp < 100))
				std::cout << "  ";
			else
				std::cout << " ";
		}
		
	}
	std::cout << std::endl;
}

void Puzzle::printSolution(std::string sStatus, std::string heuristic, uint64_t startTime)
{
	std::vector<std::map<int, int> > result;
	int steps = 0;
	std::cout << "----- N-PUZZLE -----" << std::endl;
	std::cout << "initial State:" << std::endl;
	printState(solver->initialState);
	std::cout << std::endl;
	std::cout << "goal State:" << std::endl;
	printState(solver->goalState);
	std::cout << std::endl;
	std::cout << "|" << sStatus << "|" << std::endl;
	
	State *solution = solver->solution;
	if (solution != NULL)
	{
		State tmp;
		while (solution->father != NULL)
		{
			tmp = *solution;
			// std::cout << solution->h << "|" << solution->g << std::endl;
			result.push_back(solution->state);
			solution = solution->father;
		
			// std::cout << "--" << std::endl;
			tmp.father = NULL;
			manhattanHeuristic(&tmp);
			// std::cout << tmp.h << std::endl;
			// std::cout << "---------" << std::endl;
		}
		std::cout << "____________________________________________________" << std::endl;
		printState(solver->initialState);
		std::cout << std::endl;
		while (result.empty() == false)
		{
			printState(*(result.rbegin()));
			std::cout << std::endl;
			result.pop_back();
			++steps;
		}
	}
	// State tmp;
	// while (solution->father != NULL)
	// {
	// 	tmp = *solution;
	// 	std::cout << solution->h << "|" << solution->g << std::endl;
	// 	result.push_back(solution->state);
	// 	solution = solution->father;
		
	// 	std::cout << "--" << std::endl;
	// 	tmp.father = NULL;
	// 	manhattanHeuristic(&tmp);
	// 	std::cout << tmp.h << std::endl;
	// 	std::cout << "---------" << std::endl;
	// }
	// std::cout << "____________________________________________________" << std::endl;
	// printState(solver->initialState);
	// std::cout << std::endl;
	// while (result.empty() == false)
	// {
	// 	printState(*(result.rbegin()));
	// 	std::cout << std::endl;
	// 	result.pop_back();
	// 	++steps;
	// }
		
	std::cout << "____________________________________________________" << std::endl;

	std::ofstream myFile;
	myFile.open("history.txt", std::ios::app);

	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ms -= startTime;

	if (myFile.is_open()) {
		myFile << "____________________________________________________" << std::endl;
		myFile << "Time to solution = " << ms << " ms." << std::endl;
		myFile << "Total steps to solution = " << steps << std::endl;
		myFile << "Time complexity (total states selected for OPENED queue): " << solver->nStates << std::endl;
		myFile << "Size complexity (max number of states in memory at the same time: " << solver->maxNsim << std::endl;
		myFile.close();
	}

	if (heuristic == "hamm"){
		std::cout << "\033[31mTime to solution = " << ms << " ms.\033[0m" << std::endl;
		std::cout << "\033[31mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[31mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[31mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	} else if (heuristic == "euclid"){
		std::cout << "\033[33mTime to solution = " << ms << " ms.\033[0m" << std::endl;
		std::cout << "\033[33mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[33mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[33mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	} else {
		std::cout << "\033[32mTime to solution = " << ms << " ms.\033[0m" << std::endl;
		std::cout << "\033[32mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[32mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[32mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	}
	delete(solver->solution);
	// unsigned long i = 0;
	// for(; i < solver->opened.size(); ++i)
	// {
	// 	std::cout << std::endl;
	// 	printState(solver->opened.top()->state);
	// 	std::cout << std::endl;
	// }

}

// heuristic funcs ************************************************************

void	Puzzle::manhattanHeuristic(State *state)
{
	int heuristic = 0; 
	int current = 0;
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if(state->father == NULL)
	{
		for (; i < (size * size); ++i)
		{
			// std::cout << "i = " << i << std::endl;
			// std::cout << "current pos = " << state->state.at(i) << std::endl;
			// std::cout << "goal pos = " << solver->goalState.at(i) << std::endl;
			current = abs(state->state.at(i) - solver->goalState.at(i));
			// std::cout << current << std::endl;
			// std::cout << (current / size) << "+" << (current % size) << std::endl;
			heuristic += ((current / size) + (current % size));
			// std::cout << "start heuristic: " << heuristic << std::endl;
			// std::cout << "--------------"<< std::endl;
			state->h = heuristic;
			// state->f = heuristic;
		}
	}
	else 
	{
		int tile = 0;
		heuristic = state->h;
		// std::cout << "heuristic was: " << heuristic <<std::endl;

		// std::cout << "moved tile: " << state->movedTile << std::endl;
		tile = state->reverse.at(state->movedTile);
		// std::cout << "tile: " << tile << std::endl;
		
		// printState(state->father->state);
		// std::cout << " "<< std::endl;
		// printState(state->state);
		// std::cout << "--------------"<< std::endl;
		
		current = abs(state->state.at(0) - solver->goalState.at(tile));
		// std::cout << "current0 = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		heuristic -= ((current / size) + (current % size));
		// std::cout << "heuristic - 0: " << heuristic << std::endl;
	
		current = abs(state->state.at(tile) - solver->goalState.at(tile));
		// std::cout << "currentpos = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		
		heuristic += ((current / size) + (current % size));
		
		state->h = heuristic;
		// std::cout << "heuristic: " << heuristic <<std::endl;
		// std::cout << "-------next one-------\n"<< std::endl;
	}
	
}

void Puzzle::euclidianHeuristic(State *state)
{
	int heuristic = 0; 
	int current = 0;
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if(state->father == NULL)
	{
		for (; i < (size * size); ++i)
		{
			// std::cout << "i = " << i << std::endl;
			// std::cout << "current pos = " << state->state.at(i) << std::endl;
			// std::cout << "goal pos = " << solver->goalState.at(i) << std::endl;
			current = abs(state->state.at(i) - solver->goalState.at(i));
			// std::cout << current << std::endl;
			// std::cout << (current / size) << "+" << (current % size) << std::endl;
			heuristic += int(std::sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
			// std::cout << "start heuristic: " << heuristic << std::endl;
			// std::cout << "--------------"<< std::endl;
			state->h = heuristic;
			// state->f = heuristic;
		}
	}
	else 
	{
		int tile = 0;
		heuristic = state->h;
		// std::cout << "heuristic was: " << heuristic <<std::endl;

		// std::cout << "moved tile: " << state->movedTile << std::endl;
		tile = state->reverse.at(state->movedTile);
		// std::cout << "tile: " << tile << std::endl;
		
		// printState(state->father->state);
		// std::cout << " "<< std::endl;
		// printState(state->state);
		// std::cout << "--------------"<< std::endl;
		
		current = abs(state->state.at(0) - solver->goalState.at(tile));
		// std::cout << "current0 = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		heuristic -= int(sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
		// std::cout << "heuristic - 0: " << heuristic << std::endl;
	
		current = abs(state->state.at(tile) - solver->goalState.at(tile));
		// std::cout << "currentpos = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		
		heuristic += int(sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		

		state->h = heuristic;
		// std::cout << "heuristic: " << heuristic <<std::endl;
		// std::cout << "-------next one-------\n"<< std::endl;
	}
}

void	Puzzle::hammingHeuristic(State *state)
{
	int heuristic = 0; 
	// int current = 0;
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if(state->father == NULL)
	{
		for (; i < (size * size); ++i)
		{
			// std::cout << "i = " << i << std::endl;
			// std::cout << "current pos = " << state->state.at(i) << std::endl;
			// std::cout << "goal pos = " << solver->goalState.at(i) << std::endl;
			// current = state->state.at(i) - solver->goalState.at(i);
			// std::cout << current << std::endl;
			// std::cout << (current / size) << "+" << (current % size) << std::endl;
			if (state->state.at(i) != solver->goalState.at(i))
				heuristic += 1;
			// std::cout << "start heuristic: " << heuristic << std::endl;
			// std::cout << "--------------"<< std::endl;
			state->h = heuristic;
			// state->f = heuristic;
		}
		// std::cout << "start heuristic: " << heuristic << std::endl;
	}
	else 
	{
		int tile = 0;
		heuristic = state->h;
		// std::cout << "heuristic was: " << heuristic <<std::endl;

		// std::cout << "moved tile: " << state->movedTile << std::endl;
		tile = state->reverse.at(state->movedTile);
		// std::cout << "tile: " << tile << std::endl;
		
		// printState(state->father->state);
		// std::cout << " "<< std::endl;
		// printState(state->state);
		// std::cout << "--------------"<< std::endl;
		
		if (state->state.at(0) != solver->goalState.at(tile))
		// std::cout << "current0 = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
			heuristic -= 1;
		// std::cout << "heuristic - 0: " << heuristic << std::endl;
	
		if (state->state.at(tile) != solver->goalState.at(tile))
		// std::cout << "currentpos = " << current << std::endl;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		
			heuristic += 1;
		// std::cout << (current / size) << "+" << (current % size) << std::endl;
		

		state->h = heuristic;
		// std::cout << "heuristic: " << heuristic <<std::endl;
		// std::cout << "-------next one-------\n"<< std::endl;
	}
}
