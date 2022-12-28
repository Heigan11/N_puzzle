
#include "PuzzleData.hpp"

PuzzleData::PuzzleData() {
	this->puzzleSize = 0;
	this->tiles = std::vector<int>();
}

PuzzleData::PuzzleData(int &size, std::vector<int> &newTiles) : puzzleSize(size), tiles(newTiles) { }

PuzzleData::~PuzzleData() { }

int PuzzleData::getPuzzleSize() const {
	return (this->puzzleSize);
}

std::vector<int> PuzzleData::getTiles() const {
	return (this->tiles);	
}

std::ostream& operator<<(std::ostream &stream, const PuzzleData &puzzle) {
	int n = puzzle.getPuzzleSize();
	std::vector<int> tmp = puzzle.getTiles();
	int cntr = 0;
	std::vector<int>::iterator it = tmp.begin();

	for (; it != tmp.end(); ++it) {
		stream << *it;
		cntr += 1;
		if (cntr == n) {
			if (it == (tmp.end() - 1)) {
				break ;
			}
			stream << std::endl;
			cntr = 0;
		} else {
			if (*it < 10) {
				stream << "   ";
			} else if ((*it >= 10) &&  (*it <= 99)) {
				stream << "  ";
			} else {
				stream << " ";
			}
		}
	}
	return (stream);
}