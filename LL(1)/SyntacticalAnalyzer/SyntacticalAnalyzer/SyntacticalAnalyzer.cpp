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

void ConvertStringToBool(std::string boolInStr, bool& boolVariable)
{
	if (boolInStr == "0")
		boolVariable = false;
	else if (boolInStr == "1")
		boolVariable = true;
	else
		throw std::exception("Incorrect input\nShift, Error, Stack, End must be 0 or 1");
}

void InitGuideCharactersAndShift(std::istringstream& iss, std::vector<std::string>& guideCharacters, bool& shift)
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

void InitBoolVariable(std::istringstream& iss, bool& boolVariable)
{
	std::string boolInStr = GetString(iss);
	ConvertStringToBool(boolInStr, boolVariable);
}

void InitNumberVariable(std::istringstream& iss, size_t& numVariable)
{
	std::string pointerInStr = GetString(iss);
	numVariable = std::atoi(pointerInStr.c_str()) || pointerInStr == "0" ? std::atoi(pointerInStr.c_str()) : throw std::exception("");
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

		InitNumberVariable(iss, inputData.number);
		InitSymbol(iss, inputData.symbol);
		InitGuideCharactersAndShift(iss, inputData.guideCharacters, inputData.isShift);
		InitBoolVariable(iss, inputData.isError);
		InitNumberVariable(iss, inputData.pointer);
		InitBoolVariable(iss, inputData.isStack);
		InitBoolVariable(iss, inputData.isEnd);

		inputTable.push_back(inputData);
	}
}

void InitSentence(std::ifstream& fileSentenceInput, std::string& sentence)
{
	std::getline(fileSentenceInput, sentence);
}

InputTableData GetInputDataBySymbol(std::vector<InputTableData>& inputTable, std::string symbol)
{
	for (size_t i = 0; i < inputTable.size(); i++)
	{
		if (inputTable[i].symbol == symbol)
			return inputTable[i];
	}
}

InputTableData GetInputDataByNumber(std::vector<InputTableData>& inputTable, size_t number)
{
	for (size_t i = 0; i < inputTable.size(); i++)
	{
		if (inputTable[i].number == number)
			return inputTable[i];
	}
}

bool HaveSymbolInGuide(std::vector<std::string> guideCharacters, std::string symbol)
{
	auto result = std::find(guideCharacters.begin(), guideCharacters.end(), symbol);
	return result != guideCharacters.end();
}

InputTableData GetInputDataBySymbolAndCurrentSymbol(std::vector<InputTableData>& inputTable, std::string symbol, std::string currentSymbol)
{
	InputTableData result;

	for (size_t i = 0; i < inputTable.size(); i++)
	{
		if ((inputTable[i].symbol == symbol) && (inputTable[i].guideCharacters.size() == 1))
		{
			if (HaveSymbolInGuide(inputTable[i].guideCharacters, currentSymbol))
			{
				result = inputTable[i];
			}
			else if (HaveSymbolInGuide(inputTable[i].guideCharacters, "#"))
			{
				result = inputTable[i];
			}
		}
	}

	return result;
}

InputTableData GetNewInputData(std::vector<InputTableData>& inputTable, std::string currentSymbol, size_t pointer)
{
	InputTableData result = GetInputDataByNumber(inputTable, pointer);

	if (!HaveSymbolInGuide(result.guideCharacters, currentSymbol) && result.pointer != 0)
	{
		result = GetInputDataBySymbolAndCurrentSymbol(inputTable, result.symbol, currentSymbol);
	}

	return result;
}

void RecursiveMethod(
	std::istringstream& iss,
	std::vector<InputTableData>& inputTable,
	std::vector<OutputTableData>& outputTable,
	std::stack<int>& stack,
	const InputTableData inputData,
	std::string currentSymbol)
{
	if (inputData.isEnd)
		return;

	if (inputData.isShift)
	{
		iss >> currentSymbol;
	}

	if (inputData.isStack)
	{
		size_t stackItem = inputData.number + 1;

		OutputTableData outputData = {
			inputData.number,
			Action::Add,
			stackItem,
			currentSymbol,
		};

		outputTable.push_back(outputData);

		stack.push(stackItem);

		InputTableData newInputData = GetNewInputData(inputTable, currentSymbol, inputData.pointer);
		RecursiveMethod(iss, inputTable, outputTable, stack, newInputData, currentSymbol);
	}
	else
	{
		if (inputData.pointer == 0)
		{
			size_t pointer = stack.top();
			stack.pop();

			OutputTableData outputData = {
				inputData.number,
				Action::Delete,
				pointer,
				currentSymbol,
			};

			outputTable.push_back(outputData);

			InputTableData newInputData = GetNewInputData(inputTable, currentSymbol, pointer);
			RecursiveMethod(iss, inputTable, outputTable, stack, newInputData, currentSymbol);
		}
		else
		{
			InputTableData newInputData = GetNewInputData(inputTable, currentSymbol, inputData.pointer);
			RecursiveMethod(iss, inputTable, outputTable, stack, newInputData, currentSymbol);
		}
	}
}

void MakeProcess(std::vector<InputTableData>& inputTable, std::vector<OutputTableData>& outputTable, std::string& sentence)
{
	std::string currentSymbol;
	std::istringstream iss(sentence);
	std::stack<int> stack;

	iss >> currentSymbol;
	InputTableData inputData = GetInputDataBySymbol(inputTable, currentSymbol);

	RecursiveMethod(iss, inputTable, outputTable, stack, inputData, currentSymbol);

	if (stack.empty())
	{
		std::cout << "Stack is empty. Good" << std::endl;
	}
	else
	{
		std::cout << "Stack is NOT empty. Bad" << std::endl;
	}
}

std::string ConvertActionToString(Action action)
{
	switch (action)
	{
	case Action::Add:
		return "Add";
	case Action::Delete:
		return "Delete";
	default:
		return "";
	}
}

void PrintResult(std::ofstream & fileOutput, const std::vector<OutputTableData>& outputTable)
{
	fileOutput << "Number" << TAB << "Action" << TAB << "Stack" << TAB << "CurrentSymbol" << std::endl;

	for (size_t i = 0; i < outputTable.size(); i++)
	{
		OutputTableData outputData = outputTable[i];
		fileOutput << outputData.number << TAB << ConvertActionToString(outputData.action) << TAB << outputData.stackItem << TAB << outputData.currentSymbol << std::endl;
	}
}