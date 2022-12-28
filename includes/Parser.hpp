
#pragma once

#include <exception>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <string>
#include <ctime>
#include <unistd.h>
#include <unordered_set>

#include "PuzzleData.hpp"

struct PuzzlePack
{
	std::string currentHeuristic;
	std::string currentSearch;
	PuzzleData	puzzleData;
};

class Parser
{
	friend class Puzzle;
	
	public:

		Parser();
		virtual ~Parser();

	private:

		PuzzlePack 			cmdParser(int argc, char **argv);
		PuzzlePack 			setPuzzleData(std::string &filename, std::string &heuristicType, std::string &searchType, int &size);	

		bool 				checkConversion(char *src);
		void				checkDuplicates(std::vector<int> src);
		std::vector<int> 	readPuzzleFromFile(std::string filename, int *size);
		std::vector<int> 	convertFilePuzzle(std::vector<std::string> params);
		std::vector<int> 	genRandomPuzzle(int &size);
		int 				findItem(std::vector<int> &src, int &item);
};