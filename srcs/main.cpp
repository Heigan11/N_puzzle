
#include <exception>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <string>
#include <ctime>
#include <unistd.h>

#include "PuzzleData.hpp"
#include "Parser.hpp"
#include "Runner.hpp"
#include "Puzzle.hpp"

int main(int argc, char **argv)
{

	Puzzle puzzle = Puzzle();

	if (argc == 2 && std::string(argv[1]) == "stats"){
		puzzle.printStats();
		exit(0);
	}
	if (argc == 2 && std::string(argv[1]) == "compare"){
		puzzle.printCompare();
		exit(0);
	}
	try {
		puzzle.parseData(argc, argv);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	puzzle.solvePuzzle();
}