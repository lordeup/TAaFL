#include "Visualization.h"

Visualization::Visualization(const VectorSize_t& outputMoore, const size_t size)
	: m_outputMoore(outputMoore)
	, m_size(size)
{
}

void Visualization::GraphView() const
{
	VectorString weights(m_outputMoore.size());
	VectorEdge edge(m_outputMoore.size());
	std::ofstream ofs("outputGraph.dot");

	for (size_t i = 0, x = 0, index = 0; i < (m_outputMoore.size()); ++i, ++index)
	{
		if (i % m_size == 0 && i != 0)
		{
			++x;
			index = 0;
		}

		weights[i] = SYMBOL_X + std::to_string(x);
		edge[i] = { index, m_outputMoore[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_size);

	dynamic_properties dp;
	dp.property("label", get(edge_weight, graph));
	dp.property("node_id", get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}