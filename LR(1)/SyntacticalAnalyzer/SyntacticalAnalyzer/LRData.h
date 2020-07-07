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
	std::string rule;
	std::string ch;
	size_t size;
	std::vector<Symbol> symbols;
};