#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
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
	std::string nonterminal;
	std::string character;
	size_t row = 0;
	size_t position = 0;
};

struct InputData
{
	std::string nonterminal;
	VectorString terminals;
	VectorString guideCharacters;
};
