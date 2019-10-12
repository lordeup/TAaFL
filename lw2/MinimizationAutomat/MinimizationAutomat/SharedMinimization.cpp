#include "SharedMinimization.h"

SharedMinimization::SharedMinimization(const int inputSize, const int stateCount, const VectorEdge& inputEdge, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
	, m_automat(automat)
{
}

SharedMinimization::SharedMinimization(const int inputSize, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
	, m_automat(automat)
{
}

VectorEdge SharedMinimization::GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge)
{
	VectorEdge groupOutputEdge(m_stateCount);

	for (size_t i = 0; i < groupOutputEdge.size(); ++i)
	{
		groupOutputEdge[i] = std::make_pair(inputEdge[i].second, inputEdge[i + m_stateCount].second);
	}

	return groupOutputEdge;
}

VectorEdge SharedMinimization::GettingGroupOutputEdgeMoore(const VectorInt& outputCharacter, const VectorInt& uniqueItem)
{
	VectorEdge conformityGroupEdge(m_stateCount);

	for (int i = 0; i < uniqueItem.size(); ++i)
	{
		for (int j = 0; j < outputCharacter.size(); ++j)
		{
			if (uniqueItem[i] == outputCharacter[j])
			{
				conformityGroupEdge[j] = std::make_pair(i, j);
			}
		}
	}

	return conformityGroupEdge;
}

int SharedMinimization::GetOutputStateSize()
{
	return (int)m_conformityGroupVectorPrevious.size();
}

template <typename T>
T GettingUniqueEdge(const T& groupOutputEdge, const int size)
{
	T uniqueEdge(size);

	std::copy(groupOutputEdge.begin(), groupOutputEdge.end(), uniqueEdge.begin());
	std::sort(uniqueEdge.begin(), uniqueEdge.end());
	uniqueEdge.erase(std::unique(uniqueEdge.begin(), uniqueEdge.end()), uniqueEdge.end());

	return uniqueEdge;
}

VectorEdge SharedMinimization::GettingUniqueEdgeNext(const VectorEdge& groupOutputEdge, const DualVectorInt& conformityGroupVector)
{
	VectorEdge uniqueEdge;

	for (auto it = conformityGroupVector.begin(); it != conformityGroupVector.end(); ++it)
	{
		std::unordered_set<Edge, boost::hash<Edge>> setUnique;

		for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			Edge currentEdge = groupOutputEdge[*it2];
			setUnique.insert(currentEdge);
		}

		uniqueEdge.reserve(uniqueEdge.size() + setUnique.size());
		uniqueEdge.insert(uniqueEdge.end(), setUnique.begin(), setUnique.end());
	}

	return uniqueEdge;
}

VectorEdge SharedMinimization::GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector)
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

VectorEdge SharedMinimization::GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, const VectorEdge& conformityGroupEdgeStart, DualVectorInt& conformityGroupVector)
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

				// && conformityGroupEdgeStart[i].first == conformityGroupEdgeStart[j].first
			}
		}
	}

	return conformityGroupEdge;
}

VectorEdge SharedMinimization::GetConformityPreviousGroupEdge(DualVectorInt& conformityGroupVectorPrevious, DualVectorInt& conformityGroupVector, VectorEdge& conformityGroupEdge)
{
	VectorEdge conformityPreviousGroupEdge;
	int size = m_stateCount * 2;
	VectorEdge outputState(size);

	for (size_t i = 0; i < size; ++i)
	{
		VectorEdge conformityGroupEdgeNext, conformityGroupAll;
		DualVectorInt conformityGroupVectorNext;

		if (i != 0)
		{
			VectorEdge groupOutputEdgeNext = GettingGroupOutputEdgeMealy(outputState);
			VectorEdge uniqueEdgeNext = GettingUniqueEdgeNext(groupOutputEdgeNext, conformityGroupVectorPrevious);
			conformityGroupEdgeNext = GettingConformityGroupEdgeNext(groupOutputEdgeNext, uniqueEdgeNext, conformityGroupEdge, conformityGroupVectorNext);
		}

		conformityGroupAll = i == 0 ? conformityGroupEdge : conformityGroupEdgeNext;

		for (int j = 0; j < m_stateCount; ++j)
		{
			int indexEdge = j;
			int unit = m_inputEdge[indexEdge].first;

			for (int k = 0; k < m_inputSize; ++k)
			{
				auto it = std::find_if(conformityGroupAll.begin(), conformityGroupAll.end(), [&unit](const Edge& edge) { return edge.second == unit; });

				if (it != conformityGroupAll.end())
				{
					outputState[indexEdge] = std::make_pair((*it).second, (*it).first);
				}

				if (k < m_inputSize - 1)
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

	return conformityPreviousGroupEdge;
}

void SharedMinimization::FillOutput(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge)
{
	int size = GetOutputStateSize();
	int sizeOutput = size * m_inputSize;
	m_outputState.resize(sizeOutput);

	for (int i = 0; i < size; ++i)
	{
		int indexEdge = i;
		int index = conformityGroupVectorPrevious[i][0];

		for (int j = 0; j < m_inputSize; ++j)
		{
			Edge inputEdgeFind = m_inputEdge[index];
			int unit = inputEdgeFind.first;

			auto it = std::find_if(conformityPreviousGroupEdge.begin(), conformityPreviousGroupEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

			if (it != conformityPreviousGroupEdge.end())
			{
				m_outputState[indexEdge] = std::make_pair((*it).first, inputEdgeFind.second);
			}

			if (j < m_inputSize - 1)
			{
				indexEdge += size;
				index += m_stateCount;
			}
		}
	}
}

VectorEdge SharedMinimization::Minimization()
{
	VectorInt uniqueItem;
	if (m_automat == Automat::MOORE)
	{
		uniqueItem = GettingUniqueEdge(m_outputCharacter, m_stateCount);
	}

	VectorEdge groupOutputEdge = m_automat == Automat::MEALY ? GettingGroupOutputEdgeMealy(m_inputEdge) : GettingGroupOutputEdgeMoore(m_outputCharacter, uniqueItem);

	VectorEdge uniqueEdge = GettingUniqueEdge(groupOutputEdge, m_stateCount);

	VectorEdge conformityGroupEdge = GettingConformityGroupEdge(groupOutputEdge, uniqueEdge, m_conformityGroupVector);

	VectorEdge conformityPreviousGroupEdge = GetConformityPreviousGroupEdge(m_conformityGroupVectorPrevious, m_conformityGroupVector, conformityGroupEdge);

	FillOutput(m_conformityGroupVectorPrevious, conformityPreviousGroupEdge);

	return m_outputState;
}