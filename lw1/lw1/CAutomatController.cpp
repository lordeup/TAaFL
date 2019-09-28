#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_automat = Automat::UNKNOWN;
}

void CAutomatController::ProcessingCommand()
{
	int inputSize, outputSize, stateCount;
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
		VectorInt outputState = FillOutputState(stateCount);
		std::vector<VectorInt> state;

		try
		{
			state = FillingDataMoore(inputSize, stateCount);
		}
		catch (const std::invalid_argument& error)
		{
			m_output << error.what();
			return;
		}

		CAutomatMoore automatMoore(m_output, stateCount, outputState, state);
		automatMoore.TransferAutomat();
		automatMoore.PrintInfo();
		automatMoore.GraphView();
	}
	else if (m_automat == Automat::MEALY)
	{
		VectorEdge edge = FillingDataMealy(inputSize, stateCount);

		CAutomatMealy automatMealy(m_output, inputSize, stateCount, edge);
		automatMealy.TransferAutomat();
		automatMealy.PrintInfo();
		automatMealy.GraphView();
	}
}

VectorInt CAutomatController::FillOutputState(const int stateCount)
{
	VectorInt outputCharacter(stateCount);

	for (int i = 0; i < stateCount; ++i)
	{
		m_input >> outputCharacter[i];
	}

	return outputCharacter;
}

std::vector<VectorInt> CAutomatController::FillingDataMoore(const int inputSize, const int stateCount)
{
	std::vector<VectorInt> state(inputSize);
	int number;

	for (size_t i = 0; i < state.size(); ++i)
	{
		state[i].resize(stateCount);
		for (int j = 0; j < stateCount; ++j)
		{
			m_input >> number;
			if (number >= stateCount)
			{
				throw std::invalid_argument(ERROR_WRONG_DATA);
			}
			state[i][j] = number;
		}
	}

	return state;
}

VectorEdge CAutomatController::FillingDataMealy(const int inputSize, const int stateCount)
{
	int size = inputSize * stateCount;
	VectorEdge mealyEdge(size);

	for (size_t i = 0; i < mealyEdge.size(); ++i)
	{
		m_input >> mealyEdge[i].first >> mealyEdge[i].second;
	}
	return mealyEdge;
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