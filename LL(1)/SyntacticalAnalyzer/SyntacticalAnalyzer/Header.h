#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct InputTableData
{
	size_t number = 0;
	std::string symbol;
	std::vector<std::string> guideCharacters;
	bool isShift = false;
	bool isError = true;
	size_t pointer = 0;
	bool isStack = false;
	bool isEnd = false;
};

enum class Action
{
	Add,
	Delete
};

struct OutputTableData
{
	size_t number;
	Action action;
	size_t stackItem;
	std::string currentSymbol;
};

using PairStringString = std::pair<std::string, std::string>;

const std::string TAB = "\t";
const std::string END_CHAIN = "#";
