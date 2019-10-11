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

VectorEdge CAutomatMealy::GettingGroupOutputEdge(const VectorEdge& inputEdge)
{
	VectorEdge groupOutputEdge(m_stateCount);

	for (size_t i = 0; i < groupOutputEdge.size(); ++i)
	{
		groupOutputEdge[i] = std::make_pair(inputEdge[i].second, inputEdge[i + m_stateCount].second);
	}

	return groupOutputEdge;
}

VectorEdge CAutomatMealy::GettingUniqueEdge(const VectorEdge& groupOutputEdge)
{
	VectorEdge uniqueEdge(m_stateCount);

	std::copy(groupOutputEdge.begin(), groupOutputEdge.end(), uniqueEdge.begin());
	std::sort(uniqueEdge.begin(), uniqueEdge.end());
	uniqueEdge.erase(std::unique(uniqueEdge.begin(), uniqueEdge.end()), uniqueEdge.end());

	return uniqueEdge;
}

VectorEdge CAutomatMealy::GettingUniqueEdgeNext(const VectorEdge& groupOutputEdge, const DualVectorInt& conformityGroupVector)
{
	VectorEdge uniqueEdge;
	Edge prevEdge;

	for (auto it = conformityGroupVector.begin(); it != conformityGroupVector.end(); ++it)
	{
		for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			Edge currentEdge = groupOutputEdge[*it2];

			if (it2 != (*it).begin() && currentEdge == prevEdge)
			{
				uniqueEdge.push_back(currentEdge);
			}

			prevEdge = currentEdge;
		}
	}

	return uniqueEdge;
}

VectorEdge CAutomatMealy::GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueEdge.size());

	for (int i = 0; i < uniqueEdge.size(); ++i)
	{
		for (int j = 0; j < groupOutputEdge.size(); ++j)
		{
			if (uniqueEdge[i] == groupOutputEdge[j])
			{
				conformityGroupEdge[j] = std::make_pair(i, j);
				conformityGroupVector[i].push_back(j);
			}
		}
	}

	return conformityGroupEdge;
}

VectorEdge CAutomatMealy::GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, const VectorEdge& conformityGroupEdgeStart, DualVectorInt& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueEdge.size());

	for (int i = 0; i < uniqueEdge.size(); ++i)
	{
		for (int j = 0; j < groupOutputEdge.size(); ++j)
		{
			if (uniqueEdge[i] == groupOutputEdge[j] && conformityGroupEdgeStart[i].first == conformityGroupEdgeStart[j].first)
			{
				conformityGroupEdge[j] = std::make_pair(i, j);
				conformityGroupVector[i].push_back(j);
			}
		}
	}

	return conformityGroupEdge;
}

void CAutomatMealy::MinimizationAutomat()
{
	VectorEdge groupOutputEdge = GettingGroupOutputEdge(m_inputEdge);
	VectorEdge uniqueEdge = GettingUniqueEdge(groupOutputEdge);

	DualVectorInt conformityGroupVector, conformityGroupVectorPrevious;
	VectorEdge conformityGroupEdge = GettingConformityGroupEdge(groupOutputEdge, uniqueEdge, conformityGroupVector);

	VectorEdge conformityPreviousGroupEdge;

	int size = m_stateCount * 2;
	VectorEdge outputState(size);

	for (size_t i = 0; i < m_stateCount; ++i)
	{
		VectorEdge conformityGroupEdgeNext, conformityGroupAll;
		DualVectorInt conformityGroupVectorNext;

		if (i != 0)
		{
			VectorEdge groupOutputEdgeNext = GettingGroupOutputEdge(outputState);
			VectorEdge uniqueEdgeNext = GettingUniqueEdgeNext(groupOutputEdgeNext, conformityGroupVectorPrevious);
			conformityGroupEdgeNext = GettingConformityGroupEdgeNext(groupOutputEdgeNext, uniqueEdgeNext, conformityGroupEdge, conformityGroupVectorNext);
		}

		conformityGroupAll = i == 0 ? conformityGroupEdge : conformityGroupEdgeNext;

		for (int i = 0; i < m_stateCount; ++i)
		{
			int indexEdge = i;
			int unit = m_inputEdge[indexEdge].first;

			for (int j = 0; j < m_inputSize; ++j)
			{
				auto it = std::find_if(conformityGroupAll.begin(), conformityGroupAll.end(), [&unit](const Edge& edge) { return edge.second == unit; });

				outputState[indexEdge] = std::make_pair((*it).second, (*it).first);

				if (j < m_inputSize - 1)
				{
					indexEdge += m_stateCount;
					unit = m_inputEdge[indexEdge].first;
				}
			}
		}

		if (conformityPreviousGroupEdge == conformityGroupAll)
		{
			break;
		}

		conformityPreviousGroupEdge = conformityGroupAll;
		conformityGroupVectorPrevious = i == 0 ? conformityGroupVector : conformityGroupVectorNext;
	}
}

void CAutomatMealy::PrintInfo() const
{
	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		if (i % m_stateCount == 0 && i != 0)
		{
			m_output << std::endl;
		}

		m_output << SYMBOL_Z << m_outputState[i] << SPASE;
	}
}