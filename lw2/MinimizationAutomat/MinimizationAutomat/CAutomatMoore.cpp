#include "CAutomatMoore.h"

CAutomatMoore::CAutomatMoore(std::ostream& output, const int inputSize, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
{
}

void CAutomatMoore::GraphView() const
{
	//VectorString weights(m_edge.size());
	//VectorEdge edge(m_edge.size());
	//std::ofstream ofs(OUTPUT_GRAPH_NAME);

	//for (int i = 0, x = 0, index = 0; i < m_edge.size(); ++i, ++index)
	//{
	//	if (i % m_stateCount == 0 && i != 0)
	//	{
	//		++x;
	//		index = 0;
	//	}
	//	weights[i] = SYMBOL_X + std::to_string(x) + SYMBOL_Y + std::to_string(m_edge[i].second);
	//	edge[i] = { index, m_edge[i].first };
	//}

	//Graph graph(edge.begin(), edge.end(), weights.begin(), m_stateCount);

	//dynamic_properties dp;
	//dp.property(LABEL, get(edge_weight, graph));
	//dp.property(NODE_ID, get(vertex_index, graph));

	//write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMoore::MinimizationAutomat()
{
	SharedMinimization sharedMinimization(m_inputSize, m_stateCount, m_outputCharacter, m_state, Automat::MOORE);
	//VectorInt uniqueItem = GettingUniqueItem(m_outputCharacter);
	//VectorEdge conformityGroupEdge = GettingConformityGroupEdge(m_outputCharacter, uniqueItem);
}

void CAutomatMoore::PrintInfo() const
{
	for (size_t i = 0; i < m_edge.size(); ++i)
	{
		if (i % m_stateCount == 0 && i != 0)
		{
			m_output << std::endl;
		}

		m_output << SYMBOL_Q << m_edge[i].first << SYMBOL_Y << m_edge[i].second << SPASE;
	}
}