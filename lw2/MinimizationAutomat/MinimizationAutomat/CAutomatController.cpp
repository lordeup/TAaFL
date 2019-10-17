#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_automat = Automat::UNKNOWN;
}

void CAutomatController::ProcessingCommand()
{
	size_t inputSize, outputSize, stateCount;
	std::string nameAutomat;

	m_input >> inputSize >> outputSize >> stateCount >> nameAutomat;

	try
	{
		SetAutomat(nameAutomat);
	}
	catch (const std::invalid_argument& error)
	{
		m_output << error.what();
		return;
	}

	if (m_automat == Automat::MOORE)
	{
		VectorSize_t outputCharacter;
		std::vector<VectorSize_t> state;

		try
		{
			outputCharacter = FillOutputState(stateCount);
			state = FillingDataMoore(inputSize, stateCount);
		}
		catch (const std::invalid_argument& error)
		{
			m_output << error.what();
			return;
		}

		CAutomatMoore automatMoore(m_output, inputSize, stateCount, outputCharacter, state);
		automatMoore.MinimizationAutomat();
		automatMoore.PrintInfo();
		automatMoore.GraphView();
	}
	else if (m_automat == Automat::MEALY)
	{
		VectorEdge inputEdge = FillingDataMealy(inputSize, stateCount);

		CAutomatMealy automatMealy(m_output, inputSize, stateCount, inputEdge);
		automatMealy.MinimizationAutomat();
		automatMealy.PrintInfo();
		automatMealy.GraphView();
	}
}

size_t CAutomatController::SearchNumberInStringMoore(const std::string str)
{
	std::smatch match;
	std::regex strRegex(NUMBER_REGULAR_EXPRESSION);
	size_t number;

	if (std::regex_search(str, match, strRegex))
	{
		number = std::stoi(match[0]);
	}
	else
	{
		throw std::invalid_argument(ERROR_WRONG_DATA);
	}

	return number;
}

Edge CAutomatController::SearchNumberInStringMealy(const std::string str)
{
	std::smatch match;
	std::regex strRegex(NUMBER_REGULAR_EXPRESSION);
	std::string str2;
	Edge num;

	if (std::regex_search(str, match, strRegex))
	{
		num.first = std::stoi(match[0]);
		str2 = match.suffix();
		if (std::regex_search(str2, match, strRegex))
		{
			num.second = std::stoi(match[0]);
		}
	}
	else
	{
		throw std::invalid_argument(ERROR_WRONG_DATA);
	}

	return num;
}

VectorSize_t CAutomatController::FillOutputState(const size_t stateCount)
{
	VectorSize_t outputCharacter(stateCount);
	std::string str;

	for (size_t i = 0; i < stateCount; ++i)
	{
		m_input >> str;
		outputCharacter[i] = SearchNumberInStringMoore(str);
	}

	return outputCharacter;
}

std::vector<VectorSize_t> CAutomatController::FillingDataMoore(const size_t inputSize, const size_t stateCount)
{
	std::vector<VectorSize_t> state(inputSize);
	size_t number;
	std::string str;

	for (size_t i = 0; i < state.size(); ++i)
	{
		state[i].resize(stateCount);
		for (size_t j = 0; j < stateCount; ++j)
		{
			m_input >> str;
			number = SearchNumberInStringMoore(str);
			if (number >= stateCount)
			{
				throw std::invalid_argument(ERROR_WRONG_DATA);
			}
			state[i][j] = number;
		}
	}

	return state;
}

VectorEdge CAutomatController::FillingDataMealy(const size_t inputSize, const size_t stateCount)
{
	size_t size = inputSize * stateCount;
	VectorEdge inputEdge(size);
	std::string str;

	for (size_t i = 0; i < inputEdge.size(); ++i)
	{
		m_input >> str;
		inputEdge[i] = SearchNumberInStringMealy(str);
	}
	return inputEdge;
}

void CAutomatController::SetAutomat(const std::string automat)
{
	if (automat == AUTOMAT_MOORE)
	{
		m_automat = Automat::MOORE;
	}
	else if (automat == AUTOMAT_MEALY)
	{
		m_automat = Automat::MEALY;
	}
	else
	{
		throw std::invalid_argument(ERROR_UNKNOWN_MACHINE);
	}
}