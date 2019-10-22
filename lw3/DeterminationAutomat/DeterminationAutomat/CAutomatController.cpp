#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
}

void CAutomatController::ProcessingCommand()
{
	size_t inputSize, stateCount;

	m_input >> inputSize >> stateCount;

	try
	{
		DualVectorSize_t inputState = FillingData(inputSize, stateCount);
		Automat automat(m_output, inputSize, stateCount, inputState);
		automat.Determination();
		automat.PrintInfo();
		automat.GraphView();
	}
	catch (const std::exception& error)
	{
		m_output << error.what();
	}
}

DualVectorSize_t CAutomatController::FillingData(const size_t inputSize, const size_t stateCount)
{
	DualVectorSize_t input(inputSize * stateCount);
	std::string str;

	for (size_t i = 0; i < input.size(); ++i)
	{
		m_input >> str;
		input[i] = SearchNumberInString(str);
	}

	return input;
}

VectorSize_t CAutomatController::SearchNumberInString(std::string str)
{
	std::smatch match;
	std::regex strRegex("\\d+");
	VectorSize_t numbers;

	while (std::regex_search(str, match, strRegex))
	{
		size_t num;
		try
		{
			num = std::stoi(match[0]);
		}
		catch (const std::exception&)
		{
			throw std::invalid_argument(ERROR_WRONG_DATA);
		}
		numbers.push_back(num);
		str = match.suffix();
	}

	return numbers;
}