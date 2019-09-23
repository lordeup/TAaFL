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

void CAutomatController::DataReading()
{
	std::string nameAutomat;
	std::string str;

	//	std::istringstream iss(str);

	m_input >> m_sizeInputCharacter;
	m_input >> m_sizeOutputCharacter;
	m_input >> m_countVertice;
	m_input >> nameAutomat;

	SetAutomat(nameAutomat);

	if (m_automat == Automat::MOORE)
	{
		for (int i = 0; i < m_countVertice; ++i)
		{
			std::getline(m_input, str);
		}
	}
}