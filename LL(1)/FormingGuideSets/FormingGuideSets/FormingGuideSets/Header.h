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

struct OutputData
{
	std::string nonterminal;
	std::vector<std::string> terminals;
	std::vector<std::string> guideCharacters;
};

using VectorString = std::vector<std::string>;
using PairStringBool = std::pair<std::string, bool>;
using PairStringVectorPair = std::pair<std::string, std::vector<PairStringBool>>;
using VectorInputData = std::vector<InputData>;
using VectorOutputData = std::vector<OutputData>;

const std::string SPACE = " ";
const std::string NONTERMINAL_END_SEQUENCE = "e";
const std::string TERMINAL_END_SEQUENCE = "#";