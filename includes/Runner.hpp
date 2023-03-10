
#pragma once

#include <queue>
#include <map>
#include <set>

#include "Puzzle.hpp"
#include "PuzzleData.hpp"
#include "State.hpp"

class Puzzle;

class Runner
{

	friend class Puzzle;

	public:

		Runner(Puzzle *puzzle);
		virtual ~Runner();

		void	setInitialState(const std::vector<int> &puzzle, const int &size);
		void	setGoalState();
		void	findSolution(void (Puzzle::*heuristic)(State*), std::string &search);

		// std::map<int, int>		getInitialState();
		// std::map<int, int>		getGoalSate();

	private:

		std::priority_queue<State*, std::vector<State*>, CompareStates> opened;
		// std::set<State*> closed;
		std::unordered_set<State*, HashState, HashStateCompare> closed;

		Puzzle					*puzzle;

		int						nSize;
		std::map<int, int>		goalState;
		std::map<int, int>		initialState;

		std::map<char, std::set<int> > extremePos;

		unsigned int			nStates;
		unsigned int			maxNsim;

		State 					*solution;

		void					(Puzzle::*heuristicFunc)(State*);

		void					increaseNbr(int *nbr);
		int						countInversion();

		void					generateExtremePos();

		bool					isSolvable();

		std::map<int,int>		setReverseState(std::map<int,int> toRev);

		State					*moveRight(State *state);
		State					*moveDown(State *state);
		State					*moveLeft(State *state);
		State					*moveUp(State *state);

		bool					isSolved(State *current);

		void					increaseMaxNSim();

		void					startAstar();
		void					generateNewStates(State *parent);
		void					addState(State *newState);

};
