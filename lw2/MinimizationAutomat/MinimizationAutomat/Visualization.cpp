#include "Visualization.h"

Visualization::Visualization(const VectorSize_t& outputMoore, const size_t size, Automat automat)
	: m_outputMoore(outputMoore)
	, m_size(size)
	, m_automat(automat)
{
}

Visualization::Visualization(const VectorEdge& outputMealy, const size_t size, Automat automat)
	: m_outputMealy(outputMealy)
	, m_size(size)
	, m_automat(automat)
{
}

void Visualization::GraphView() const
{
	size_t size = m_automat == Automat::MEALY ? m_outputMealy.size() : m_outputMoore.size();
	VectorString weights(size);
	VectorEdge edge(size);
	std::ofstream ofs("outputGraph.dot");

	for (size_t i = 0, x = 0, index = 0; i < size; ++i, ++index)
	{
		if (i % m_size == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		if (m_automat == Automat::MEALY)
		{
			weights[i] = SYMBOL_X + std::to_string(x) + SYMBOL_Y + std::to_string(m_outputMealy[i].second);
			edge[i] = { index, m_outputMealy[i].first };
		}
		else
		{
			weights[i] = SYMBOL_X + std::to_string(x);
			edge[i] = { index, m_outputMoore[i] };
		}
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_size);

	dynamic_properties dp;
	dp.property("label", get(edge_weight, graph));
	dp.property("node_id", get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}