
#include "Puzzle.hpp"

Puzzle::Puzzle() {
	this->heuristic = "";
	this->search = "";
	this->puzzle = PuzzleData();
	this->parser = Parser();
	this->solver = new Solver(this);
}

Puzzle::Puzzle(PuzzleData &newPuzzle, std::string &heuristicType, std::string &searchType) : 
	heuristic(heuristicType), search(searchType), puzzle(newPuzzle) {
	this->parser = Parser();
}

Puzzle::~Puzzle() {
	delete this->solver;
}

void Puzzle::printStats(){

	int countRuns = 0;
	int bestTime = -1;
	int worstTime = -1;

	std::string line;
 
    std::ifstream in("history.txt");
    if (in.is_open()) {
        while (getline(in, line))
        {
            if (line == "____________________________________________________"){
				countRuns++;
			}
			if (line.find("Time to solution = ") != std::string::npos) {
				std::string output = std::regex_replace(line, std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));
				int time = stoi(output);
				if (bestTime > time || bestTime == -1){
					bestTime = time;
				}
				if (worstTime < time || worstTime == -1){
					worstTime = time;
				}
			}	
        }
    }
    in.close();
	std::cout << "Count of runs = " << countRuns << std::endl;
	std::cout << "Best time = " << bestTime << " ms" << std::endl;
	std::cout << "Worst time = " << worstTime << " ms" << std::endl;
} 

void Puzzle::printCompare(){

	int bestTimeManhtn = -1;
	int bestTimeEuclid = -1;
	int bestTimeHamm = -1;

	std::string line;
 
    std::ifstream in("history.txt");
    if (in.is_open()) {
        while (getline(in, line))
        {
			if (line.find("Time to solution = ") != std::string::npos) {
				std::string output = std::regex_replace(line, std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));
				int time = stoi(output);
				if (line.find("hamm") != std::string::npos){
					if (bestTimeHamm > time || bestTimeHamm == -1){
						bestTimeHamm = time;
					}
				} else if (line.find("euclid") != std::string::npos){
					if (bestTimeEuclid > time || bestTimeEuclid == -1){
						bestTimeEuclid = time;
					}
				} else {
					if (bestTimeManhtn > time || bestTimeManhtn == -1){
						bestTimeManhtn = time;
					}
				}
			}	
        }
    }
    in.close();
	if (bestTimeManhtn > 0 && bestTimeEuclid > 0 && bestTimeHamm > 0){
		std::cout << "Comparation of speed depend on heuristic: " << std::endl;
		std::cout << "Manhattan = " << 100 << "%" << std::endl;
		std::cout << "Euclidean = " << (int)((double) bestTimeManhtn / (double) bestTimeEuclid * 100)  << "%" << std::endl;
		std::cout << "Hamm = " << (int)((double) bestTimeManhtn / (double) bestTimeHamm * 100) << "%" << std::endl;
	} else {
		std::cout << "You must put to history all types of heuristic(hamm, euclid, manhtn)" << std::endl;
	}
} 

std::string	Puzzle::getHeuristic() {
	return (this->heuristic);
}

std::string Puzzle::getSearch() {
	return (this->search);
}

PuzzleData Puzzle::getPuzzle() {
	return (this->puzzle);	
}

void	Puzzle::parseData(int argc, char **argv) {

	PuzzlePack tmp;
	tmp = this->parser.cmdParser(argc, argv);
	this->heuristic = tmp.currentHeuristic;
	this->search = tmp.currentSearch;
	this->puzzle = tmp.puzzleData;
}

void Puzzle::solvePuzzle() {
	solver->setInitialState(this->puzzle.getTiles(), this->puzzle.getPuzzleSize());
	solver->setGoalState();
	solve();
}

void Puzzle::clearStates() {
	while (solver->opened.size()) {
		delete solver->opened.top();
		solver->opened.pop();
	}
	while (solver->closed.size()) {
		delete (*(solver->closed.begin()));
		solver->closed.erase(solver->closed.begin());
	}	
}

void Puzzle::solve() {
	uint64_t startTime;

	if (solver->isSolvable() == false) {
		printSolution("unsolvable", "-", 0);
		clearStates();
		return ;
	}
	
	void (Puzzle::*heuristicFunc)(State *state);
	if (heuristic == "manhtn") {
		heuristicFunc = &Puzzle::manhattanHeuristic;
	} else if (heuristic == "euclid") {
		heuristicFunc = &Puzzle::euclidianHeuristic;
	} else {
		heuristicFunc = &Puzzle::hammingHeuristic;
	}
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();	
	solver->findSolution(heuristicFunc, search);
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
	
	for (; i < (size * size); ++i) {	
		it = state.begin();
		for (; it != state.end(); ++it) {
			if (i == it->second) {
				tmp = it->first;
			}
		}	
		std::cout << tmp;
		cntr += 1;
		if (cntr == size){
			if (i == ((size * size) - 1)){
				break ;
			}
			std::cout << std::endl;
			cntr = 0;
		} else {
			if (tmp < 10){
				std::cout << "   ";
			} else if ((tmp > 9) &&  (tmp < 100)) {
				std::cout << "  ";
			} else {
				std::cout << " ";
			}
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

	if (sStatus == "unsolvable"){
		return;
	}
	
	State *solution = solver->solution;
	if (solution != NULL) {
		State tmp;
		while (solution->father != NULL) {
			tmp = *solution;
			result.push_back(solution->state);
			solution = solution->father;
			tmp.father = NULL;
			manhattanHeuristic(&tmp);
		}
		std::cout << "____________________________________________________" << std::endl;
		printState(solver->initialState);
		std::cout << std::endl;
		while (result.empty() == false) {
			printState(*(result.rbegin()));
			std::cout << std::endl;
			result.pop_back();
			++steps;
		}
	}
		
	std::cout << "____________________________________________________" << std::endl;

	std::ofstream myFile;
	myFile.open("history.txt", std::ios::app);

	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ms -= startTime;

	if (myFile.is_open()) {
		myFile << "____________________________________________________" << std::endl;
		myFile << "Time to solution = " << ms << " ms. Is a " << heuristic << std::endl;
		myFile << "Total steps to solution = " << steps << std::endl;
		myFile << "Time complexity (total states selected for OPENED queue): " << solver->nStates << std::endl;
		myFile << "Size complexity (max number of states in memory at the same time: " << solver->maxNsim << std::endl;
		myFile.close();
	}
	if (heuristic == "hamm"){
		std::cout << "\033[31mTime to solution = " << ms << " ms. Is a hamm.\033[0m" << std::endl;
		std::cout << "\033[31mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[31mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[31mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	} else if (heuristic == "euclid"){
		std::cout << "\033[33mTime to solution = " << ms << " ms. Is a euclid.\033[0m" << std::endl;
		std::cout << "\033[33mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[33mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[33mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	} else {
		std::cout << "\033[32mTime to solution = " << ms << " ms. Is a manhtn.\033[0m" << std::endl;
		std::cout << "\033[32mTotal steps to solution = " << steps << "\033[0m" << std::endl;
		std::cout << "\033[32mTime complexity (total states selected for OPENED queue): " << solver->nStates << "\033[0m" << std::endl;
		std::cout << "\033[32mSize complexity (max number of states in memory at the same time: " << solver->maxNsim << "\033[0m" << std::endl;
	}
	delete(solver->solution);
}

void	Puzzle::manhattanHeuristic(State *state) {
	int heuristic = 0; 
	int current = 0;
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if (state->father == NULL) {
		for (; i < (size * size); ++i) {
			current = abs(state->state.at(i) - solver->goalState.at(i));
			heuristic += ((current / size) + (current % size));
			state->h = heuristic;
		}
	} else {
		int tile = 0;
		heuristic = state->h;
		tile = state->reverse.at(state->movedTile);
		current = abs(state->state.at(0) - solver->goalState.at(tile));
		heuristic -= ((current / size) + (current % size));
		current = abs(state->state.at(tile) - solver->goalState.at(tile));
		heuristic += ((current / size) + (current % size));	
		state->h = heuristic;
	}
	
}

void Puzzle::euclidianHeuristic(State *state) {
	int heuristic = 0; 
	int current = 0;
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if (state->father == NULL) {
		for (; i < (size * size); ++i) {
			current = abs(state->state.at(i) - solver->goalState.at(i));
			heuristic += int(std::sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
			state->h = heuristic;
		}
	} else {
		int tile = 0;
		heuristic = state->h;
		tile = state->reverse.at(state->movedTile);		
		current = abs(state->state.at(0) - solver->goalState.at(tile));
		heuristic -= int(sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
		current = abs(state->state.at(tile) - solver->goalState.at(tile));
		heuristic += int(sqrt(((current / size) * (current / size)) + ((current % size) * (current % size))));
		state->h = heuristic;
	}
}

void	Puzzle::hammingHeuristic(State *state) {
	int heuristic = 0; 
	unsigned long i = 1; 
	unsigned long size = puzzle.getPuzzleSize();
	if (state->father == NULL) {
		for (; i < (size * size); ++i) {
			if (state->state.at(i) != solver->goalState.at(i))
				heuristic += 1;
			state->h = heuristic;
		}
	} else {
		int tile = 0;
		heuristic = state->h;
		tile = state->reverse.at(state->movedTile);
		if (state->state.at(0) != solver->goalState.at(tile)){
			heuristic -= 1;
		}
		if (state->state.at(tile) != solver->goalState.at(tile)){
			heuristic += 1;
		}
		state->h = heuristic;
	}
}
