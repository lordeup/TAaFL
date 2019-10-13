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
	//VectorString weights(m_outputState.size());
	//VectorEdge edge(m_outputState.size());
	//std::ofstream ofs(OUTPUT_GRAPH_NAME);

	//for (int i = 0, x = 0, index = 0; i < m_outputState.size(); ++i, ++index)
	//{
	//	if (i % m_outputStateSize == 0 && i != 0)
	//	{
	//		++x;
	//		index = 0;
	//	}
	//	weights[i] = SYMBOL_X + std::to_string(x) + SYMBOL_Y + std::to_string(m_outputState[i].second);
	//	edge[i] = { index, m_outputState[i].first };
	//}

	//Graph graph(edge.begin(), edge.end(), weights.begin(), m_outputStateSize);

	//dynamic_properties dp;
	//dp.property(LABEL, get(edge_weight, graph));
	//dp.property(NODE_ID, get(vertex_index, graph));

	//write_graphviz_dp(ofs, graph, dp);

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