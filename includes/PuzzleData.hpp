
#pragma once

#include <vector>
#include <string>
#include <iostream>

class PuzzleData 
{
	public:
		
		PuzzleData();
		PuzzleData(int &newSize,  std::vector<int> &newTiles);
		virtual ~PuzzleData();

		int						getPuzzleSize() const;
		std::vector<int>		getTiles() const;

	private:
	
		int						puzzleSize;
		std::vector<int>		tiles;

};

std::ostream& operator<<(std::ostream &stream, const PuzzleData &puzzle);