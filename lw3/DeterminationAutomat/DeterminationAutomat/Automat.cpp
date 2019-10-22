#include "Automat.h"

Automat::Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const DualVectorSize_t& inputState)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputState(inputState)
{
}

void Automat::GraphView() const
{
	//Visualization visualization(m_outputState, m_outputStateSize);
	//visualization.GraphView();
}

void Automat::Determination()
{
}

void Automat::PrintInfo() const
{
	//for (size_t i = 0; i < m_outputState.size(); ++i)
	//{
	//	if (i % m_outputStateSize == 0 && i != 0)
	//	{
	//		m_output << std::endl;
	//	}

	//	m_output << SYMBOL_S << m_outputState[i].first << SYMBOL_Y << m_outputState[i].second << SPASE;
	//}
}
