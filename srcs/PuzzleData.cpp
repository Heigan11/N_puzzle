
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
	int size = puzzle.getPuzzleSize();
	std::vector<int> tmpTiles = puzzle.getTiles();
	int cntr = 0;
	std::vector<int>::iterator it = tmpTiles.begin();

	for (; it != tmpTiles.end(); ++it) {
		stream << *it;
		cntr += 1;
		if (cntr == size) {
			if (it == (tmpTiles.end() - 1)) {
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