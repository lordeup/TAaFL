#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct InputTableData
{
	std::string symbol;
	std::vector<std::string> guideCharacters;
	bool isShift = false;
	bool isError = true;
	size_t pointer = 0;
	bool isStack = false;
	bool isEnd = false;
};

struct OutputTableData
{
};
