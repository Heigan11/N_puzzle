
#pragma once

#include <map>
#include <string>

struct State
{
		std::map<int, int>		state;
		std::map<int, int>		reverse;

		int						g;
		int						h;

		State					*father;
		int						movedTile;

};

struct HashState
{
	size_t operator() (State *state) const
	{
		std::string src = "";
		std::map<int, int>::iterator it = state->state.begin();
		for (; it != state->state.end(); ++it)
			src += it->second;
		return (std::hash<std::string>()(src));
	}
};

struct HashStateCompare
{
	bool operator()(State *first, State *second) const
	{
		unsigned long i = 0;
		for(; i < first->state.size(); ++i)
		{
			if (first->state.at(i) != second->state.at(i))
			{
				return (false);
			}
		}
		return (true);
	}
};

struct CompareStates
{

	std::string		search;

	CompareStates() : search("default") {};
	CompareStates(std::string initS) : search(initS) {}

	bool 			operator()(State *first, State *second)
	{
		// где g — это стоимость достижения s из начального состояния
		// (число ходов от начального состояния до s),
		// h — рациональный компонент агента (эвристика)
		if (search == "uniform")
		{
			return (first->g > second->g);
		}
		else if (search == "greedy")
		{
			return (first->h > second->h);
		}
		else // default
		{
			return ((first->g + first->h) > (second->g + second->h));
		}

		return (false);
	}

};

