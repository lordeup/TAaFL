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

void CAutomatController::FillingData()
{
	m_edges.resize(m_sizeInputCharacter);

	for (int i = 0; i < m_edges.size(); ++i)
	{
		m_edges[i].resize(m_countVertice);
		for (int j = 0; j < m_countVertice; ++j)
		{
			m_input >> m_edges[i][j];
		}
	}
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

		FillingData();
	}
	else if (m_automat == Automat::MEALY)
	{
		FillingData();
	}
}