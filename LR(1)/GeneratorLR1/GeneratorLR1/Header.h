#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

const std::string TAB = "\t";
const std::string NONTERMINAL_END_SEQUENCE = "e";
const std::string TERMINAL_END_SEQUENCE = "#";
const std::string OK = "Ok";
const std::string SHIFT = "S";
const std::string ROLL_UP = "R";

using VectorString = std::vector<std::string>;

struct TableData
{
	size_t count;
	size_t position;
	std::string character;
};

struct InputData
{
	std::string nonterminal;
	VectorString terminals;
	VectorString guideCharacters;
};
