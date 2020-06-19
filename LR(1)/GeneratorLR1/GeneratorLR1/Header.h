#pragma once
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

const std::string TAB = "\t";
const std::string SPACE = " ";
const std::string NONTERMINAL_END_SEQUENCE = "e";
const std::string TERMINAL_END_SEQUENCE = "#";
const std::string OK = "Ok";
const std::string SHIFT = "S";
const std::string ROLL_UP = "R";

struct InputData
{
	std::string nonterminal;
	std::vector<std::string> terminals;
};

struct OutputDataGuideSets
{
	std::string nonterminal;
	std::vector<std::string> terminals;
	std::vector<std::string> guideCharacters;
};

struct TableData
{
	std::string nonterminal;
	std::string character;
	size_t row = 0;
	size_t position = 0;
};