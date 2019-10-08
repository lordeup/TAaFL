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
	//VectorString weights(m_outputState.size());
	//VectorEdge edge(m_outputState.size());
	//std::ofstream ofs(OUTPUT_GRAPH_NAME);

	//for (int i = 0, x = 0, index = 0; i < m_outputState.size(); ++i, ++index)
	//{
	//	if (i % m_uniqueEdge.size() == 0 && i != 0)
	//	{
	//		++x;
	//		index = 0;
	//	}
	//	weights[i] = SYMBOL_X + std::to_string(x);
	//	edge[i] = { index, m_outputState[i] };
	//}

	//Graph graph(edge.begin(), edge.end(), weights.begin(), m_uniqueEdge.size());

	//dynamic_properties dp;
	//dp.property(LABEL, get(edge_weight, graph));
	//dp.property(NODE_ID, get(vertex_index, graph));

	//write_graphviz_dp(ofs, graph, dp);
}

void CAutomatMealy::MinimizationAutomat()
{
	m_groupStateEdge.resize(m_stateCount);
	m_uniqueEdge.resize(m_stateCount);
	m_conformityGroupEdge.resize(m_stateCount);
	VectorInt conformityGroupEdge(m_stateCount);

	for (size_t i = 0; i < m_groupStateEdge.size(); ++i)
	{
		m_groupStateEdge[i] = { m_inputEdge[i].second, m_inputEdge[i + m_stateCount].second };
	}

	std::copy(m_groupStateEdge.begin(), m_groupStateEdge.end(), m_uniqueEdge.begin());
	std::sort(m_uniqueEdge.begin(), m_uniqueEdge.end());
	m_uniqueEdge.erase(std::unique(m_uniqueEdge.begin(), m_uniqueEdge.end()), m_uniqueEdge.end());

	for (int i = 0; i < m_uniqueEdge.size(); ++i)
	{
		for (int j = 0; j < m_groupStateEdge.size(); ++j)
		{
			if (m_groupStateEdge[j] == m_uniqueEdge[i])
			{
				conformityGroupEdge[j] = i;
				m_conformityGroupEdge[j] = { i, j };
			}
		}
	}

	//std::sort(m_conformityGroupEdge.begin(), m_conformityGroupEdge.end());

	int size = m_stateCount * 2;
	VectorEdge outputState(size);
	VectorInt outputStateInt(size);

	for (int i = 0; i < m_stateCount; ++i)
	{
		int indexEdge = i;
		int index = m_inputEdge[indexEdge].first;

		for (int j = 0; j < m_inputSize; ++j)
		{
			auto it = std::find_if(m_conformityGroupEdge.begin(), m_conformityGroupEdge.end(), [&index](const Edge& edge) { return edge.second == index; });

			outputStateInt[indexEdge] = (*it).first;

			if (j < m_inputSize - 1)
			{
				indexEdge += m_stateCount;
				index = m_inputEdge[indexEdge].first;
			}
		}
	}

	m_output << std::endl;
}

void CAutomatMealy::PrintInfo() const
{
	m_output << AUTOMAT_MEALY << std::endl;

	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		if (i % m_uniqueEdge.size() == 0 && i != 0)
		{
			m_output << std::endl;
		}

		m_output << SYMBOL_Z << m_outputState[i] << SPASE;
	}
}