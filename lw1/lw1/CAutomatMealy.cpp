#include "CAutomatMealy.h"

CAutomatMealy::CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, const VectorEdge& edge)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_mealyEdge(edge)
{
}

void CAutomatMealy::GraphView() const
{
	VectorString weights(m_edge.size());
	VectorEdge edge(m_edge.size());
	std::ofstream ofs(OUTPUT_GRAPH_NAME);

	for (int i = 0, x = 1, index = 0; i < m_edge.size(); ++i, ++index)
	{
		if (i % m_copyEdge.size() == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = SYMBOL_X + std::to_string(x);
		edge[i] = { index, m_edge[i].first };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_copyEdge.size());

	dynamic_properties dp;
	dp.property(LABEL, get(edge_weight, graph));
	dp.property(NODE_ID, get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMealy::TransferAutomat()
{
	m_copyEdge.resize(m_mealyEdge.size());

	std::copy(m_mealyEdge.begin(), m_mealyEdge.end(), m_copyEdge.begin());
	std::sort(m_copyEdge.begin(), m_copyEdge.end());
	m_copyEdge.erase(std::unique(m_copyEdge.begin(), m_copyEdge.end()), m_copyEdge.end());

	m_edge.resize(m_inputSize * m_copyEdge.size());

	for (size_t i = 0; i < m_copyEdge.size(); ++i)
	{
		size_t indexEdge = i;
		int index = m_copyEdge[i].first;

		for (int j = 0; j < m_inputSize; ++j)
		{
			auto it = std::find(m_copyEdge.begin(), m_copyEdge.end(), m_mealyEdge[index]);
			int indexFindCopy = int(std::distance(m_copyEdge.begin(), it));
			m_edge[indexEdge] = { indexFindCopy, m_copyEdge[i].second };

			if (j < m_inputSize - 1)
			{
				indexEdge += m_copyEdge.size();
				index += m_stateCount;
			}
		}
	}
}

void CAutomatMealy::PrintInfo() const
{
	for (size_t i = 0; i < m_copyEdge.size(); ++i)
	{
		if (i == 0)
		{
			m_output << AUTOMAT_MOORE << std::endl
					 << TAB;
		}

		m_output << SYMBOL_Z << std::to_string(i) << SLASH << SYMBOL_Y << std::to_string(m_copyEdge[i].second) << TAB;
	}

	m_output << std::endl;

	for (size_t i = 0, x = 0; i < m_edge.size(); ++i)
	{
		if (i % m_copyEdge.size() == 0)
		{
			++x;
			m_output << std::endl
					 << SYMBOL_X << std::to_string(x) << TAB;
		}

		m_output << SYMBOL_Z << std::to_string(m_edge[i].first) << TAB;
	}
}