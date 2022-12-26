/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchelste <bchelste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 20:29:03 by bchelste          #+#    #+#             */
/*   Updated: 2022/10/03 16:07:39 by bchelste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <string>
#include <ctime>
#include <unistd.h>


#include "PuzzleData.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
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

	try
	{
		puzzle.parseData(argc, argv);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	// std::cout << "--data from input--" << std::endl;
	// std::cout << "heuristic type: " << (puzzle).getHeuristic() << std::endl;
	// std::cout << "search type: " << (puzzle).getSearch() << std::endl;
	// std::cout << "puzzle data: \n" << (puzzle).getPuzzle() << std::endl;
	
	puzzle.solvePuzzle();
	
}