#pragma once
#include <string>
#include <vector>

enum class State
{
	Shift,
	Convolution,
	None,
	Ok
};

struct Symbol
{
	State state;
	size_t number;
};

struct LRData
{
	size_t number;
	std::string ch;
	std::vector<Symbol> symbols;
};

struct GuideSetsData
{
	std::string nonterminal;
	std::vector<std::string> terminals;
};