#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <iostream>

struct InputTableData
{
	size_t number;
	std::string symbol;
	std::vector<std::string> guideCharacters;
	bool isShift = false;
	bool isError = true;
	size_t pointer = 0;
	bool isStack = false;
	bool isEnd = false;
};

enum Action
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

const std::string TAB = "\t";
