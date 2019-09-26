#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_sizeInputCharacter = 0;
	m_sizeOutputCharacter = 0;
	m_countVertice = 0;
	m_automat = Automat::UNKNOWN;
}

void CAutomatController::DataReading()
{
	std::string nameAutomat;

	m_input >> m_sizeInputCharacter;
	m_input >> m_sizeOutputCharacter;
	m_input >> m_countVertice;
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

		PrintInfoTransferMoore();
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

		PrintInfoTransferMealy();
	}
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

bool CAutomatController::IsFillingDataMoore()
{
	m_outputCharacter.resize(m_countVertice);

	for (int i = 0; i < m_countVertice; ++i)
	{
		m_input >> m_outputCharacter[i];
	}

	m_state.resize(m_sizeInputCharacter);
	int number;

	for (size_t i = 0; i < m_state.size(); ++i)
	{
		m_state[i].resize(m_countVertice);
		for (int j = 0; j < m_countVertice; ++j)
		{
			m_input >> number;
			if (number >= m_countVertice)
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

void CAutomatController::PrintInfoTransferMoore()
{
	size_t lineFeed = m_edge.size() / m_sizeInputCharacter;
	for (size_t i = 0; i < m_edge.size(); ++i)
	{
		if (i % lineFeed == 0 && i != 0)
		{
			m_output << std::endl;
		}
		m_output << m_edge[i].first << " " << m_edge[i].second << "\t";
	}
}

bool CAutomatController::IsFillingDataMealy()
{
	int size = m_sizeInputCharacter * m_countVertice;
	m_mealy_edge.resize(size);
	int state, outputCharacter;

	for (size_t i = 0; i < m_mealy_edge.size(); ++i)
	{
		m_input >> state >> outputCharacter;
		m_mealy_edge[i] = { state, outputCharacter };
	}
	return true;
}

void CAutomatController::TransferAutomatMoore()
{
	std::vector<Edge> copyMealyEdge(m_mealy_edge);

	std::copy(m_mealy_edge.begin(), m_mealy_edge.end(), copyMealyEdge.begin());
	std::sort(copyMealyEdge.begin(), copyMealyEdge.end());
	copyMealyEdge.erase(std::unique(copyMealyEdge.begin(), copyMealyEdge.end()), copyMealyEdge.end());

	size_t size = m_sizeInputCharacter * copyMealyEdge.size();
	m_mealy_edge_output.resize(size);

	for (int i = 0; i < copyMealyEdge.size(); ++i)
	{
		int index = copyMealyEdge[i].first;
		int nextIndex = index + m_countVertice;
		int ind = copyMealyEdge.size() + i;

		for (int j = 0; j < copyMealyEdge.size(); ++j)
		{
		}

		auto it = std::find(copyMealyEdge.begin(), copyMealyEdge.end(), m_mealy_edge[index]);
		int indexFindCopy = int(std::distance(copyMealyEdge.begin(), it));
		m_mealy_edge_output[i] = { indexFindCopy, copyMealyEdge[i].second };

		auto itNext = std::find(copyMealyEdge.begin(), copyMealyEdge.end(), m_mealy_edge[nextIndex]);
		int indexFindCopyNext = int(std::distance(copyMealyEdge.begin(), itNext));
		m_mealy_edge_output[ind] = { indexFindCopyNext, copyMealyEdge[i].second };
	}
}

void CAutomatController::PrintInfoTransferMealy()
{
	size_t lineFeed = m_mealy_edge_output.size() / m_sizeInputCharacter;
	for (size_t i = 0; i < m_mealy_edge_output.size(); ++i)
	{
		if (i % lineFeed == 0 && i != 0)
		{
			m_output << std::endl;
		}
		m_output << m_mealy_edge_output[i].first << "\t";
	}
}
