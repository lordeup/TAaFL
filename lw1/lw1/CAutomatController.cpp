#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_inputSize = 0;
	m_outputSize = 0;
	m_stateCount = 0;
	m_automat = Automat::UNKNOWN;
}

void CAutomatController::DataReading()
{
	std::string nameAutomat;

	m_input >> m_inputSize;
	m_input >> m_outputSize;
	m_input >> m_stateCount;
	m_input >> nameAutomat;

	SetAutomat(nameAutomat);

	if (m_automat == Automat::MOORE)
	{
		if (IsFillingDataMoore())
		{
			TransferAutomatMealy();
		}
		else
		{
			m_output << ERROR_WRONG_DATA;
		}
	}
	else if (m_automat == Automat::MEALY)
	{
		if (IsFillingDataMealy())
		{
			TransferAutomatMoore();
		}
		else
		{
			m_output << ERROR_WRONG_DATA;
		}
	}

	PrintInfo(m_edge);
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
		m_automat = Automat::UNKNOWN;
	}
}

void CAutomatController::PrintInfo(const EdgeVector& edge) const
{
	size_t lineFeed = edge.size() / m_inputSize;
	for (size_t i = 0; i < edge.size(); ++i)
	{
		if (i % lineFeed == 0 && i != 0)
		{
			m_output << std::endl;
		}

		if (m_automat == Automat::MOORE)
		{
			m_output << edge[i].first << " " << edge[i].second << "\t";
		}
		else if (m_automat == Automat::MEALY)
		{
			m_output << edge[i].first << "\t";
		}
	}
}

bool CAutomatController::IsFillingDataMoore()
{
	m_outputCharacter.resize(m_stateCount);

	for (int i = 0; i < m_stateCount; ++i)
	{
		m_input >> m_outputCharacter[i];
	}

	m_state.resize(m_inputSize);
	int number;

	for (size_t i = 0; i < m_state.size(); ++i)
	{
		m_state[i].resize(m_stateCount);
		for (int j = 0; j < m_stateCount; ++j)
		{
			m_input >> number;
			if (number >= m_stateCount)
			{
				return false;
			}
			m_state[i][j] = number;
		}
	}
	return true;
}

void CAutomatController::TransferAutomatMealy()
{
	for (size_t i = 0; i < m_state.size(); ++i)
	{
		for (size_t j = 0; j < m_outputCharacter.size(); ++j)
		{
			m_edge.push_back({ m_state[i][j], m_outputCharacter[m_state[i][j]] });
		}
	}
}

bool CAutomatController::IsFillingDataMealy()
{
	int size = m_inputSize * m_stateCount;
	m_mealyEdge.resize(size);
	int state, outputCharacter;

	for (size_t i = 0; i < m_mealyEdge.size(); ++i)
	{
		m_input >> state >> outputCharacter;
		m_mealyEdge[i] = { state, outputCharacter };
	}
	return true;
}

void CAutomatController::TransferAutomatMoore()
{
	EdgeVector copyMealyEdge(m_mealyEdge);

	std::copy(m_mealyEdge.begin(), m_mealyEdge.end(), copyMealyEdge.begin());
	std::sort(copyMealyEdge.begin(), copyMealyEdge.end());
	copyMealyEdge.erase(std::unique(copyMealyEdge.begin(), copyMealyEdge.end()), copyMealyEdge.end());

	size_t size = m_inputSize * copyMealyEdge.size();
	m_edge.resize(size);

	for (size_t i = 0; i < copyMealyEdge.size(); ++i)
	{
		size_t indexEdge = i;
		int index = copyMealyEdge[i].first;

		for (int j = 0; j < m_inputSize; ++j)
		{
			auto it = std::find(copyMealyEdge.begin(), copyMealyEdge.end(), m_mealyEdge[index]);
			int indexFindCopy = int(std::distance(copyMealyEdge.begin(), it));
			m_edge[indexEdge] = { indexFindCopy, copyMealyEdge[i].second };

			if (j < m_inputSize - 1)
			{
				indexEdge += copyMealyEdge.size();
				index += m_stateCount;
			}
		}
	}
}