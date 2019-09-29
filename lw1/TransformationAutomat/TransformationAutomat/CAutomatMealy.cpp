#include "CAutomatMealy.h"

CAutomatMealy::CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, const VectorEdge& inputEdge)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
{
}

void CAutomatMealy::GraphView() const
{
	VectorString weights(m_outputState.size());
	VectorEdge edge(m_outputState.size());
	std::ofstream ofs(OUTPUT_GRAPH_NAME);

	for (int i = 0, x = 1, index = 0; i < m_outputState.size(); ++i, ++index)
	{
		if (i % m_uniqueEdge.size() == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = SYMBOL_X + std::to_string(x);
		edge[i] = { index, m_outputState[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_uniqueEdge.size());

	dynamic_properties dp;
	dp.property(LABEL, get(edge_weight, graph));
	dp.property(NODE_ID, get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMealy::TransferAutomat()
{
	m_uniqueEdge.resize(m_inputEdge.size());

	std::copy(m_inputEdge.begin(), m_inputEdge.end(), m_uniqueEdge.begin());
	std::sort(m_uniqueEdge.begin(), m_uniqueEdge.end());
	m_uniqueEdge.erase(std::unique(m_uniqueEdge.begin(), m_uniqueEdge.end()), m_uniqueEdge.end());

	m_outputState.resize(m_inputSize * m_uniqueEdge.size());

	for (int i = 0; i < m_uniqueEdge.size(); ++i)
	{
		size_t indexEdge = i;
		int index = m_uniqueEdge[i].first;

		for (int j = 0; j < m_inputSize; ++j)
		{
			auto it = std::find(m_uniqueEdge.begin(), m_uniqueEdge.end(), m_inputEdge[index]);
			m_outputState[indexEdge] = int(std::distance(m_uniqueEdge.begin(), it));

			if (j < m_inputSize - 1)
			{
				indexEdge += m_uniqueEdge.size();
				index += m_stateCount;
			}
		}
	}
}

void CAutomatMealy::PrintInfo() const
{
	for (size_t i = 0; i < m_uniqueEdge.size(); ++i)
	{
		if (i == 0)
		{
			m_output << AUTOMAT_MOORE << std::endl
					 << TAB;
		}

		m_output << SYMBOL_Z << i << SLASH << SYMBOL_Y << m_uniqueEdge[i].second << TAB;
	}

	m_output << std::endl;

	for (size_t i = 0, x = 0; i < m_outputState.size(); ++i)
	{
		if (i % m_uniqueEdge.size() == 0)
		{
			++x;
			m_output << std::endl
					 << SYMBOL_X << x << TAB;
		}

		m_output << SYMBOL_Z << m_outputState[i] << TAB;
	}
}