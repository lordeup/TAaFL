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

bool CAutomatController::IsFillingData()
{
	m_state.resize(m_sizeInputCharacter);
	int number;

	for (int i = 0; i < m_state.size(); ++i)
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
		m_outputCharacter.resize(m_countVertice);

		for (int i = 0; i < m_countVertice; ++i)
		{
			m_input >> m_outputCharacter[i];
		}

		if (IsFillingData())
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
		if (IsFillingData())
		{
			TransferAutomatMoore();
		}
		else
		{
			m_output << ERROR_WRONG_DATA;
		}
	}

	PrintInfo();
}

void CAutomatController::PrintInfo()
{
	for (int i = 0; i < m_edge.size(); ++i)
	{
		if (i == m_edge.size() / 2)
		{
			m_output << std::endl;
		}
		m_output << m_edge[i].first << " " << m_edge[i].second << "\t";
	}
}

void CAutomatController::TransferAutomatMealy()
{
	for (int i = 0; i < m_state.size(); ++i)
	{
		for (int j = 0; j < m_outputCharacter.size(); ++j)
		{
			m_edge.push_back({ m_state[i][j], m_outputCharacter[m_state[i][j]] });
		}
	}
}

void CAutomatController::TransferAutomatMoore()
{
}