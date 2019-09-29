#include "CAutomatMoore.h"

CAutomatMoore::CAutomatMoore(std::ostream& output, const int stateCount, const VectorInt& outputState, const std::vector<VectorInt>& state)
	: m_output(output)
	, m_stateCount(stateCount)
	, m_outputState(outputState)
	, m_state(state)
{
}

void CAutomatMoore::GraphView() const
{
	VectorString weights(m_edge.size());
	VectorEdge edge(m_edge.size());
	std::ofstream ofs(OUTPUT_GRAPH_NAME);

	for (size_t i = 0, x = 1, index = 0; i < m_edge.size(); ++i, ++index)
	{
		if (i % m_stateCount == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = SYMBOL_X + std::to_string(x) + SLASH + SYMBOL_Y + std::to_string(m_edge[i].second);
		edge[i] = { index, m_edge[i].first };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_stateCount);

	dynamic_properties dp;
	dp.property(LABEL, get(edge_weight, graph));
	dp.property(NODE_ID, get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMoore::TransferAutomat()
{
	for (size_t i = 0; i < m_state.size(); ++i)
	{
		for (size_t j = 0; j < m_outputState.size(); ++j)
		{
			m_edge.push_back({ m_state[i][j], m_outputState[m_state[i][j]] });
		}
	}
}

void CAutomatMoore::PrintInfo() const
{
	for (int i = 0; i < m_stateCount; ++i)
	{
		if (i == 0)
		{
			m_output << AUTOMAT_MEALY << std::endl
					 << TAB;
		}

		m_output << SYMBOL_S << i << TAB;
	}

	m_output << std::endl;

	for (size_t i = 0, x = 0; i < m_edge.size(); ++i)
	{
		if (i % m_stateCount == 0)
		{
			++x;
			m_output << std::endl
					 << SYMBOL_X << x << TAB;
		}

		m_output << SYMBOL_S << m_edge[i].first << SLASH << SYMBOL_Y << m_edge[i].second << TAB;
	}
}