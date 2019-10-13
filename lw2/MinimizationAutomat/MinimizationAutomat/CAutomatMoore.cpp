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
	//VectorString weights(m_outputStateMoore.size());
	//VectorEdge edge(m_outputStateMoore.size());
	//std::ofstream ofs(OUTPUT_GRAPH_NAME);

	//for (int i = 0, x = 0, index = 0; i < m_outputStateMoore.size(); ++i, ++index)
	//{
	//	if (i % m_outputStateSize == 0 && i != 0)
	//	{
	//		++x;
	//		index = 0;
	//	}
	//	weights[i] = SYMBOL_X + std::to_string(x);
	//	edge[i] = { index, m_outputStateMoore[i] };
	//}

	//Graph graph(edge.begin(), edge.end(), weights.begin(), m_outputStateSize);

	//dynamic_properties dp;
	//dp.property(LABEL, get(edge_weight, graph));
	//dp.property(NODE_ID, get(vertex_index, graph));

	//write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMoore::MinimizationAutomat()
{
	SharedMinimization sharedMinimization(m_inputSize, m_stateCount, m_outputCharacter, m_state, Automat::MOORE);
	m_outputStateMoore = sharedMinimization.MinimizationMoore();
	m_outputCharacterMoore = sharedMinimization.GetOutputCharacterMoore();
	m_outputStateSize = sharedMinimization.GetOutputStateSize();
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

		m_output << SYMBOL_Q << m_outputStateMoore[i] << SPASE;
	}
}