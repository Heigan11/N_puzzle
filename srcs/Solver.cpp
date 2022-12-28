
#include "Solver.hpp"

Solver::Solver(Puzzle *_puzzle) : puzzle(_puzzle), nSize(0), nStates(0), maxNsim(0),solution(NULL), heuristicFunc(NULL) { }

Solver::~Solver() { }

void Solver::setInitialState(const std::vector<int> &puzzle, const int &size) {
	nSize = size;
	std::vector<int>::const_iterator it = puzzle.begin();
	for (int i = 0; it != puzzle.end(); ++it, ++i) {
		initialState.emplace(*it, i);
	}
	maxNsim +=1;	
}

void Solver::setGoalState() {
	int nbr = 1;
	int finish = (nSize % 2 == 0) ? (nSize / 2) : (nSize / 2 + 1);
	
	for (int i = 0; i < finish; ++i) {
		for (int j = i; j < (nSize - i); ++j) {
			goalState.emplace(nbr , ((nSize * i) + j));
			increaseNbr(&nbr);
		}
		for (int j = (i + 1); j < (nSize - i); ++j) {
			goalState.emplace( nbr, ((j * nSize) + (nSize - i - 1)));
			increaseNbr(&nbr);
		}
		for (int j = (i + 1); j < (nSize - i); ++j) {
			goalState.emplace(nbr, (((nSize - i - 1) * nSize) + (nSize - j - 1)));
			increaseNbr(&nbr);
		}
		for (int j = (i + 1); j < (nSize -i - 1); ++j) {
			goalState.emplace(nbr, (((nSize - j - 1) * nSize) + i));
			increaseNbr(&nbr);
		}	
	}
	maxNsim += 1;;
	generateExtremePos();
}

std::map<int,int> Solver::setReverseState(std::map<int,int> toRev) {
	std::map<int,int> reverse;
	std::map<int,int>::iterator it = toRev.begin();
	for (; it != toRev.end(); ++it) {
		reverse.emplace(it->second, it->first);
	}
	return (reverse);
}

void Solver::findSolution(void (Puzzle::*heuristic)(State*), std::string &search) {	
	CompareStates comp = CompareStates(search);
	heuristicFunc = heuristic;
	opened = std::priority_queue<State*, std::vector<State*>, CompareStates>(comp);
	closed.clear();
	State *start = new State();
	start->state = initialState;
	start->reverse = setReverseState(start->state);
	start->father = NULL;
	start->g = 0;
	(puzzle->*heuristicFunc)(start);
	start->movedTile = -1;
	opened.push(start);
	nStates += 1;
	startAstar();
}

void Solver::increaseNbr(int *nbr) {
	*nbr += 1;
	if (*nbr == (nSize * nSize)) {
		*nbr = 0;
	}
}

int Solver::countInversion() {
	int missplaced = 0;
	unsigned long totalSize = nSize * nSize;
	for(unsigned long i = 1; i < totalSize; ++i) {
		for (unsigned long j =  1; j < totalSize; ++j) {
			if ((initialState[i] < initialState[j]) && (goalState[i] > goalState[j])) {
				missplaced += 1;
			}	
		}	
	}
	return (missplaced);
}

void Solver::generateExtremePos() {
	int i;
	int tmp;
	std::set<int> positions;
	
	i = 0;
	for(;i < nSize; ++i) {
		positions.emplace(i);
	}
	extremePos.emplace('u', positions);
	positions.clear();
	
	// move down
	i = nSize * (nSize - 1);
	for(;i < (nSize * nSize); ++i) {
		positions.emplace(i);
	}
	extremePos.emplace('d', positions);
	positions.clear();

	//move right
	i = 1;
	for (;i <= nSize; ++i) {
		tmp = (nSize * i) - 1;
		positions.emplace(tmp);
	}
	extremePos.emplace('r', positions);
	positions.clear();

	//move left
	i = 1;
	for (; i <= nSize; ++i) {
		tmp = nSize * (i - 1);
		positions.emplace(tmp);
	}
	extremePos.emplace('l', positions);
}

bool Solver::isSolvable() {
	int inversions = countInversion();
	bool result = false;

	if (nSize % 2 == 1) {
		result = inversions % 2 == 0;
	} else if (nSize % 2 == 0) {
		int tmp = initialState[0] / nSize;
		if ((tmp % 2) == 0) {
			result = inversions % 2 == 0;
		} else {
			result = inversions % 2 == 1;
		}
	}
	return (result);
}

State *Solver::moveRight(State *oldState) {
	int zeroPos = oldState->state.at(0);
	int victimValue;
	int swapped;
	
	if (extremePos.at('r').find(zeroPos) != extremePos.at('r').end()) {
		return (NULL);
	}
	
	State *newState = new State();
	newState->father = oldState;
	newState->reverse = oldState->reverse;
	newState->state = oldState->state;

	victimValue = newState->reverse.at(zeroPos + 1);
	swapped = newState->state.at(victimValue);
	newState->state.at(victimValue) = newState->state.at(0);
	newState->state.at(0) = swapped;

	swapped = newState->reverse.at(zeroPos);
	newState->reverse.at(zeroPos) = newState->reverse.at(zeroPos + 1);
	newState->reverse.at(zeroPos + 1) = swapped;
	
	newState->movedTile = zeroPos;
	newState->g = oldState->g + 1;
	newState->h = oldState->h;
	(puzzle->*heuristicFunc)(newState);

	return (newState);
}

State *Solver::moveDown(State *oldState) {
	int zeroPos = oldState->state.at(0);
	int victimValue;
	int swapped;
	
	if (extremePos.at('d').find(zeroPos) != extremePos.at('d').end()) {
		return (NULL);
	}
	
	State *newState = new State();
	newState->father = oldState;
	newState->reverse = oldState->reverse;
	newState->state = oldState->state;

	victimValue = newState->reverse.at(zeroPos + nSize);

	swapped = newState->state.at(victimValue);
	newState->state.at(victimValue) = newState->state.at(0);
	newState->state.at(0) = swapped;

	swapped = newState->reverse.at(zeroPos);
	newState->reverse.at(zeroPos) = newState->reverse.at(zeroPos + nSize);
	newState->reverse.at(zeroPos + nSize) = swapped;
	
	newState->movedTile = zeroPos;
	newState->g = oldState->g + 1;
	newState->h = oldState->h;
	(puzzle->*heuristicFunc)(newState);
	
	return (newState);
}

State *Solver::moveLeft(State *oldState)
{
	int zeroPos = oldState->state.at(0);
	int victimValue;
	int swapped;
	
	if (extremePos.at('l').find(zeroPos) != extremePos.at('l').end()) {
		return (NULL);
	}
	
	State *newState = new State();
	newState->father = oldState;
	newState->reverse = oldState->reverse;
	newState->state = oldState->state;

	victimValue = newState->reverse.at(zeroPos - 1);

	swapped = newState->state.at(victimValue);
	newState->state.at(victimValue) = newState->state.at(0);
	newState->state.at(0) = swapped;

	swapped = newState->reverse.at(zeroPos);
	newState->reverse.at(zeroPos) = newState->reverse.at(zeroPos - 1);
	newState->reverse.at(zeroPos - 1) = swapped;
	
	newState->movedTile = zeroPos;
	newState->g = oldState->g + 1;
	newState->h = oldState->h;
	(puzzle->*heuristicFunc)(newState);
	
	return (newState);
}

State *Solver::moveUp(State *oldState)
{
	int zeroPos = oldState->state.at(0);
	int victimValue;
	int swapped;
	
	if (extremePos.at('u').find(zeroPos) != extremePos.at('u').end()) {
		return (NULL);
	}
	
	State *newState = new State();
	newState->father = oldState;
	newState->reverse = oldState->reverse;
	newState->state = oldState->state;

	victimValue = newState->reverse.at(zeroPos - nSize);

	swapped = newState->state.at(victimValue);
	newState->state.at(victimValue) = newState->state.at(0);
	newState->state.at(0) = swapped;

	swapped = newState->reverse.at(zeroPos);
	newState->reverse.at(zeroPos) = newState->reverse.at(zeroPos - nSize);
	newState->reverse.at(zeroPos - nSize) = swapped;
	
	newState->movedTile = zeroPos;
	newState->g = oldState->g + 1;
	newState->h = oldState->h;
	(puzzle->*heuristicFunc)(newState);
	
	return (newState);
}

bool Solver::isSolved(State *current) {
	return (current->state == goalState);
	// if (current->state == goalState) {
	// 	return (true);
	// }
	// return (false);
}

void Solver::increaseMaxNSim() {
	unsigned int tmp = opened.size() + closed.size();
	if (tmp > maxNsim) {
		maxNsim = tmp;
	}
}

void Solver::startAstar()
{
	State *current = NULL;

	while (this->opened.empty() == false) {
		increaseMaxNSim();	
		current = opened.top();
		opened.pop();
		if (isSolved(current) == true) {
			solution = current;
			return ;
		}
		else if (closed.find(current) == closed.end()) {
			closed.emplace(current);
			generateNewStates(current);	
		} else {
			delete current;
		}
	}
}

void Solver::generateNewStates(State *parent) {
	State *newState;
	newState = moveRight(parent);
	addState(newState);
	newState = NULL;
	newState = moveDown(parent);
	addState(newState);
	newState = NULL;
	newState = moveLeft(parent);
	addState(newState);
	newState = NULL;
	newState = moveUp(parent);
	addState(newState);
	newState = NULL;
}

void Solver::addState(State *newState) {
	if (newState != NULL) {
		if (closed.find(newState) == closed.end()) {
			opened.push(newState);
			nStates += 1;
		} else {
			delete newState;
		}	
	}
}