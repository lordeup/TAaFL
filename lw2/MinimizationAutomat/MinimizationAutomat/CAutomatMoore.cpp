#include "CAutomatMoore.h"

CAutomatMoore::CAutomatMoore(std::ostream& output, const int inputSize, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
	, m_outputStateSize(0)
{
}

void CAutomatMoore::GraphView() const
{
	Visualization visualization(m_outputStateMoore, m_outputStateSize, Automat::MOORE);
	visualization.GraphView();
}

void CAutomatMoore::MinimizationAutomat()
{
	Minimization minimization(m_inputSize, m_stateCount, m_outputCharacter, m_state, Automat::MOORE);
	m_outputStateMoore = minimization.MinimizationMoore();
	m_outputCharacterMoore = minimization.GetOutputCharacterMoore();
	m_outputStateSize = minimization.GetOutputStateSize();
}

void CAutomatMoore::PrintInfo() const
{
	for (size_t i = 0; i < m_outputCharacterMoore.size(); ++i)
	{
		m_output << SYMBOL_Y << m_outputCharacterMoore[i] << SPASE;
	}

	for (size_t i = 0; i < m_outputStateMoore.size(); ++i)
	{
		if (i % m_outputStateSize == 0)
		{
			m_output << std::endl;
		}

		m_output << SYMBOL_Z << m_outputStateMoore[i] << SPASE;
	}
}