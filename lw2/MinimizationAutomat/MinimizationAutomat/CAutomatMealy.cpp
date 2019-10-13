#include "CAutomatMealy.h"

CAutomatMealy::CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, const VectorEdge& inputEdge)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
	, m_outputStateSize(0)
{
}

void CAutomatMealy::GraphView() const
{
	Visualization visualization(m_outputState, m_outputStateSize, Automat::MEALY);
	visualization.GraphView();
}

void CAutomatMealy::MinimizationAutomat()
{
	SharedMinimization sharedMinimization(m_inputSize, m_stateCount, m_inputEdge, Automat::MEALY);
	m_outputState = sharedMinimization.MinimizationMealy();
	m_outputStateSize = sharedMinimization.GetOutputStateSize();
}

void CAutomatMealy::PrintInfo() const
{
	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		if (i % m_outputStateSize == 0 && i != 0)
		{
			m_output << std::endl;
		}

		m_output << SYMBOL_S << m_outputState[i].first << SYMBOL_Y << m_outputState[i].second << SPASE;
	}
}