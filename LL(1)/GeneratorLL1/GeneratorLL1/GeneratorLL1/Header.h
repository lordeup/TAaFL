#pragma once
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

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

struct OutputData
{
	std::string symbol;
	std::vector<std::string> guideCharacters;
	bool isShift = false;
	bool isError = true;
	size_t pointer = 0;
	bool isStack = false;
	bool isEnd = false;
};

const std::string TAB = "\t";
const std::string SPACE = " ";
const std::string NONTERMINAL_END_SEQUENCE = "e";
const std::string TERMINAL_END_SEQUENCE = "#";
