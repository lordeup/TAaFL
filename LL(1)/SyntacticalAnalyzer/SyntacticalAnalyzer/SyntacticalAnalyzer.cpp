#include "SyntacticalAnalyzer.h"

std::string GetString(std::istringstream& iss)
{
	std::string str;
	iss >> str;
	return str;
}

void InitSymbol(std::istringstream& iss, std::string& symbol)
{
	symbol = GetString(iss);
}

void ConvertStringToBool(std::string boolInStr, bool boolVariable)
{
	if (boolInStr == "0")
		boolVariable = true;
	else if (boolInStr == "1")
		boolVariable = false;
	else
		throw std::exception("Incorrect input\nShift, Error, Stack, End must be 0 or 1");
}

void InitGuideCharactersAndShift(std::istringstream& iss, std::vector<std::string>& guideCharacters, bool shift)
{
	std::string str;
	while (iss >> str)
	{
		if (std::atoi(str.c_str()) || str == "0")
		{
			ConvertStringToBool(str, shift);
			break;
		}
		else
			guideCharacters.push_back(str);
	}
}

void InitBoolVariable(std::istringstream& iss, bool boolVariable)
{
	std::string boolInStr = GetString(iss);
	ConvertStringToBool(boolInStr, boolVariable);
}

void InitPointer(std::istringstream& iss, size_t& pointer)
{
	std::string pointerInStr = GetString(iss);
	pointer = std::atoi(pointerInStr.c_str()) || pointerInStr == "0" ? std::atoi(pointerInStr.c_str()) : throw std::exception("");
}

void InitInputTable(std::ifstream& fileTableInput, std::vector<InputTableData>& inputTable)
{
	std::string line;

	// read header
	std::getline(fileTableInput, line);

	while (std::getline(fileTableInput, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputTableData inputData;

		// read number
		iss >> str;

		InitSymbol(iss, inputData.symbol);
		InitGuideCharactersAndShift(iss, inputData.guideCharacters, inputData.isShift);
		InitBoolVariable(iss, inputData.isError);
		InitPointer(iss, inputData.pointer);
		InitBoolVariable(iss, inputData.isStack);
		InitBoolVariable(iss, inputData.isEnd);

		inputTable.push_back(inputData);
	}
}

void InitSentence(std::ifstream& fileSentenceInput, std::string& sentence)
{
	std::getline(fileSentenceInput, sentence);
}

void MakeProcess(std::vector<InputTableData>& inputTable, std::vector<OutputTableData>& outputTable, std::string& sentence)
{
	std::string currentSymbol;
	std::istringstream iss(sentence);
	std::stack<int> stack;
}