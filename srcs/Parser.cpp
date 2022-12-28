
#include "Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

PuzzlePack Parser::cmdParser(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::invalid_argument("very few options were given!");
	}
	std::string filename = "";
	std::string heuristic = "";
	std::string search = "";
	int puzzleSize = 0;

	for (int i = 1; i < argc; ++i)
	{
		std::string tmp = std::string(argv[i]);
		if (tmp == "-h")
		{
			i += 1;
			if (i < argc)
			{
				if (!(heuristic.empty()))
				{
					throw std::invalid_argument("specify only one type of heuristic metric");
				}
				std::string res = std::string(argv[i]);
				if ((res == "man") || (res == "euc") || (res == "ham"))
				{
					heuristic = res;
				}
				else
				{
					throw std::invalid_argument("we don't know how to calculate this type of heuristic metric yet");
				}
			}
		}
		else if (tmp == "-n")
		{
			if (puzzleSize != 0)
			{
				throw std::invalid_argument("specify the correct size of the puzzle");
			}
			i += 1;
			if (i < argc)
			{
				if (!checkConversion(argv[i]))
				{
					throw std::invalid_argument("the size of the puzzle must be a number");
				}
				puzzleSize = std::stoi(std::string(argv[i]));
				if (puzzleSize < 3)
				{
					throw std::invalid_argument("specify a puzzle size larger than 3");
				}
			}
		}
		else if (tmp == "-s")
		{
			i += 1;
			if (i < argc)
			{
				if (!(search.empty()))
				{
					throw std::invalid_argument("specify one way to calculate the metric (h + g || h || g)");
				}
				std::string res = std::string(argv[i]);
				if ((res == "default") || (res == "uniform") || (res == "greedy"))
				{
					search = res;
				}
				else
				{
					throw std::invalid_argument("we don't know how to calculate this type metric yet!");
				}
			}
		}
		else
		{
			if (filename.empty())
			{
				filename = tmp;
			}
			else
			{
				throw std::invalid_argument("specify only one puzzle file");
			}
		}
	}
	return (setPuzzleData(filename, heuristic, search, puzzleSize));
}

PuzzlePack Parser::setPuzzleData(std::string &filename, std::string &heuristicType, std::string &searchType, int &size)
{
	PuzzlePack pack;

	std::vector<int> currentPuzzle;
	int filePuzzleSize = 0;

	if ((filename.empty()) && (size > 0))
	{
		currentPuzzle = genRandomPuzzle(size);
		pack.puzzleData = PuzzleData(size, currentPuzzle);
	}
	else if (!(filename.empty()) && (size == 0))
	{
		currentPuzzle = readPuzzleFromFile(filename, &filePuzzleSize);
		checkDuplicates(currentPuzzle);
		pack.puzzleData = PuzzleData(filePuzzleSize, currentPuzzle);
	}
	else
	{
		throw std::invalid_argument("we couldn't create a puzzle");
	}
	pack.currentHeuristic = "man";
	if (!(heuristicType.empty()))
	{
		pack.currentHeuristic = heuristicType;
	}
	pack.currentSearch = "default";
	if (!(searchType.empty()))
	{
		pack.currentSearch = searchType;
	}
	return (pack);
}

bool Parser::checkConversion(char *src)
{
	for (int i = 0; src[i] != '\0'; ++i)
	{
		if (!isdigit(src[i]))
		{
			return (false);
		}
	}
	return (true);
}

void Parser::checkDuplicates(std::vector<int> src)
{
	std::unordered_set<int> seen;
	std::vector<int>::iterator it = src.begin();
	for (; it < src.end(); ++it)
	{
		seen.insert(*it);
	}
	if (seen.size() == 0)
	{
		throw std::invalid_argument("there was empty puzzle in file");
	}
	if (seen.size() != src.size())
	{
		throw std::invalid_argument("the puzzle file is not valid (tiles are duplicated)");
	}
}

std::vector<int> Parser::readPuzzleFromFile(std::string filename, int *size)
{
	int fd;
	if ((fd = open(filename.c_str(), O_RDONLY)) <= 0)
	{
		throw std::invalid_argument("we couldn't open the puzzle file " + filename);
	}

	std::vector<int> result;
	std::vector<std::string> result2;
	char buf[1];
	int ret;
	int flag = 0;

	std::string token;
	read(fd, buf, 1);
	for (ret = 1; ret > 0; ret = read(fd, buf, 1))
	{
		if ((buf[0] == '#') && (flag == 0))
		{
			flag = 1;
		}
		else if ((buf[0] == '\n') && (flag == 1))
		{
			flag = 0;
		}
		if (flag == 1)
		{
			continue;
		}
		if ((buf[0] == ' ') || (buf[0] == '\r') || (buf[0] == '\f') ||
			(buf[0] == '\n') || (buf[0] == '\t') || (buf[0] == '\v'))
		{
			if (token.length() > 0)
			{
				result2.push_back(token);
				token.clear();
			}
		}
		else
		{
			token += buf[0];
		}
	}
	close(fd);
	result = convertFilePuzzle(result2);
	*size = result.front();
	result.erase(result.begin());
	return (result);
}

std::vector<int> Parser::convertFilePuzzle(std::vector<std::string> params)
{
	std::vector<int> retParam;
	std::vector<std::string>::iterator it = params.begin();
	for (; it != params.end(); ++it)
	{
		for (unsigned long i = 0; i < (*it).size(); ++i)
		{
			if (!isdigit((*it)[i]))
			{
				throw std::invalid_argument("tile should be a number");
			}
		}
		retParam.push_back(std::stoi(*it));
	}
	return (retParam);
}

std::vector<int> Parser::genRandomPuzzle(int &size)
{
	std::vector<int> result;
	srand(time(0));
	int random;
	int n = size * size;

	for (int i = 0; i < n; ++i)
	{
		while (true)
		{
			random = rand() % n;
			if (findItem(result, random) == 0)
			{
				break;
			}
		}
		result.push_back(random);
	}
	return (result);
}

int Parser::findItem(std::vector<int> &src, int &item)
{
	std::vector<int>::iterator it = src.begin();
	for (; it != src.end(); ++it)
	{
		if (*it == item)
		{
			return (1);
		}
	}
	return (0);
}
