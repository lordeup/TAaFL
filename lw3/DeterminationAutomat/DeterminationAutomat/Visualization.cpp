#include "Visualization.h"

Visualization::Visualization(const VectorSize_t& output, const size_t size)
	: m_output(output)
	, m_size(size)
{
}

void Visualization::GraphView() const
{
	size_t size = m_output.size();
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

		weights[i] = SYMBOL_X + std::to_string(x);
		edge[i] = { index, m_output[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_size);

	dynamic_properties dp;
	dp.property("label", get(edge_weight, graph));
	dp.property("node_id", get(vertex_index, graph));

	write_graphviz_dp(ofs, graph, dp);
}