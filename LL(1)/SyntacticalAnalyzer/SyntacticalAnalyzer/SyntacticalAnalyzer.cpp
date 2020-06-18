#include "SyntacticalAnalyzer.h"

std::string GetString(std::istringstream& iss)
{
	std::string str;
	iss >> str;
	return str;
}

int ParseNumber(const std::string str)
{
	try
	{
		return std::stoi(str);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Invalid argument: " + str);
	}
}

bool GetBoolToInt(const int number)
{
	if (number == 0)
	{
		return false;
	}
	else if (number == 1)
	{
		return true;
	}
	else
	{
		throw std::exception("Incorrect input. Must be 0 or 1");
	}
}

std::vector<std::string> GetFillVector(std::istringstream& iss)
{
	std::string str;
	std::vector<std::string> vec;
	while (iss >> str)
	{
		vec.push_back(str);
	}
	return vec;
}

bool InitBoolVariable(std::istringstream& iss)
{
	int number = ParseNumber(GetString(iss));
	return GetBoolToInt(number);
}

void InitInputTable(std::ifstream& fileTableInput, std::vector<InputTableData>& inputTable)
{
	std::string line;
	std::getline(fileTableInput, line);

	while (std::getline(fileTableInput, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputTableData inputData;

		inputData.number = ParseNumber(GetString(iss));
		inputData.symbol = GetString(iss);
		inputData.isShift = InitBoolVariable(iss);
		inputData.isError = InitBoolVariable(iss);
		inputData.pointer = ParseNumber(GetString(iss));
		inputData.isStack = InitBoolVariable(iss);
		inputData.isEnd = InitBoolVariable(iss);
		inputData.guideCharacters = GetFillVector(iss);

		inputTable.push_back(inputData);
	}
}

void InitSentence(std::ifstream& fileSentenceInput, std::string& sentence)
{
	std::getline(fileSentenceInput, sentence);
}

bool HaveSymbolInGuide(const std::vector<std::string>& guideCharacters, const std::string symbol)
{
	return std::find(guideCharacters.begin(), guideCharacters.end(), symbol) != guideCharacters.end();
}

InputTableData GetInputDataBySymbolAndCurrentSymbol(std::vector<InputTableData>& inputTable, const std::string symbol, const std::string currentSymbol)
{
	auto it = std::find_if(inputTable.begin(), inputTable.end(), [&](const InputTableData& data) {
		return data.symbol == symbol && data.guideCharacters.size() == 1 && (HaveSymbolInGuide(data.guideCharacters, currentSymbol) || HaveSymbolInGuide(data.guideCharacters, "#"));
	});

	if (it == inputTable.end())
	{
		throw std::invalid_argument("Error. Wrong character: " + currentSymbol);
	}

	return *it;
}

InputTableData GetNewInputData(std::vector<InputTableData>& inputTable, std::string currentSymbol, size_t pointer)
{
	auto it = std::find_if(inputTable.begin(), inputTable.end(), [&](const InputTableData& data) { return data.number == pointer; });

	if (it == inputTable.end())
	{
		throw std::exception("Error. Not find in input table number");
	}

	InputTableData result = *it;

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
	std::stack<size_t>& stack,
	const InputTableData inputData,
	std::string currentSymbol)
{
	if (inputData.isEnd)
	{
		return;
	}

	if (inputData.isShift)
	{
		iss >> currentSymbol;
	}

	if (inputData.isStack)
	{
		size_t stackItem = inputData.number + 1;

		outputTable.push_back({ inputData.number, Action::Add, stackItem, currentSymbol });

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

			outputTable.push_back({ inputData.number, Action::Delete, pointer, currentSymbol });

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
	std::stack<size_t> stack;

	currentSymbol = GetString(iss);

	auto it = std::find_if(inputTable.begin(), inputTable.end(), [&](const InputTableData& data) { return data.symbol == currentSymbol; });

	if (it == inputTable.end())
	{
		throw std::exception("Error. Not find in input table symbol");
	}

	RecursiveMethod(iss, inputTable, outputTable, stack, *it, currentSymbol);

	std::cout << (stack.empty() ? "Stack is empty. Good" : "Stack is NOT empty. Bad") << std::endl;
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

void PrintResult(std::ofstream& fileOutput, const std::vector<OutputTableData>& outputTable)
{
	fileOutput << "Number" << TAB << "Action" << TAB << "Stack" << TAB << "CurrentSymbol" << std::endl;

	for (size_t i = 0; i < outputTable.size(); ++i)
	{
		OutputTableData outputData = outputTable[i];
		fileOutput << outputData.number << TAB << ConvertActionToString(outputData.action) << TAB << outputData.stackItem << TAB << outputData.currentSymbol << std::endl;
	}
}