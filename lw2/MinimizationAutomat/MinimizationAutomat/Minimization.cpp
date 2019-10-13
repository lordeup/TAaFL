#include "Minimization.h"

Minimization::Minimization(const int inputSize, const int stateCount, const VectorEdge& inputEdge, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
	, m_automat(automat)
{
}

Minimization::Minimization(const int inputSize, const int stateCount, const VectorInt& outputCharacter, const DualVectorInt& state, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
	, m_automat(automat)
{
}

VectorEdge Minimization::GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge)
{
	VectorEdge groupOutputEdge(m_stateCount);

	for (size_t i = 0; i < groupOutputEdge.size(); ++i)
	{
		groupOutputEdge[i] = std::make_pair(inputEdge[i].second, inputEdge[i + m_stateCount].second);
	}

	return groupOutputEdge;
}

VectorEdge Minimization::GettingGroupOutputEdgeMoore(const VectorInt& outputCharacter, const VectorInt& uniqueItem, DualVectorInt& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueItem.size());

	for (int i = 0; i < uniqueItem.size(); ++i)
	{
		for (int j = 0; j < outputCharacter.size(); ++j)
		{
			if (uniqueItem[i] == outputCharacter[j])
			{
				conformityGroupEdge[j] = std::make_pair(i, j);
				conformityGroupVector[i].push_back(j);
			}
		}
	}

	return conformityGroupEdge;
}

int Minimization::GetOutputStateSize() const
{
	return (int)m_conformityGroupVectorPrevious.size();
}

VectorInt Minimization::GetOutputCharacterMoore() const
{
	return m_outputCharacterMoore;
}

VectorEdge Minimization::GettingUniqueMealy(const VectorEdge& groupOutputEdge, const int size)
{
	VectorEdge uniqueEdge(size);

	std::copy(groupOutputEdge.begin(), groupOutputEdge.end(), uniqueEdge.begin());
	std::sort(uniqueEdge.begin(), uniqueEdge.end());
	uniqueEdge.erase(std::unique(uniqueEdge.begin(), uniqueEdge.end()), uniqueEdge.end());

	return uniqueEdge;
}

VectorInt Minimization::GettingUniqueMoore(const VectorInt& groupOutputEdge)
{
	VectorInt uniqueEdge;
	std::set<int> setUnique;

	for (const auto& group : groupOutputEdge)
	{
		setUnique.insert(group);
	}

	uniqueEdge.reserve(uniqueEdge.size() + setUnique.size());
	uniqueEdge.insert(uniqueEdge.end(), setUnique.begin(), setUnique.end());

	return uniqueEdge;
}

VectorEdge Minimization::GettingUniqueEdgeNext(const VectorEdge& groupOutputEdge, const DualVectorInt& conformityGroupVector)
{
	VectorEdge uniqueEdge;

	for (auto it = conformityGroupVector.begin(); it != conformityGroupVector.end(); ++it)
	{
		std::set<Edge> setUnique;

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

VectorEdge Minimization::GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector)
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

VectorEdge Minimization::GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueEdge.size());

	for (int i = 0, index = 0, sum = 0; i < uniqueEdge.size(); ++i, ++index)
	{
		for (int j = 0; j < m_conformityGroupVectorPrevious.size(); ++j)
		{
			int size = (int)m_conformityGroupVectorPrevious[index].size();
			for (int k = 0; k < size; ++k)
			{
				int unit = m_conformityGroupVectorPrevious[index][k];

				if (uniqueEdge[i] == groupOutputEdge[unit])
				{
					conformityGroupEdge[unit] = std::make_pair(i, unit);
					conformityGroupVector[i].push_back(unit);
				}
			}

			sum += (int)conformityGroupVector[i].size();
			sum == size ? sum = 0 : --index;

			break;
		}
	}

	std::sort(conformityGroupVector.begin(), conformityGroupVector.end(), [](const auto& a, const auto& b) {
		return *(a.begin()) < *(b.begin());
	});

	return conformityGroupEdge;
}

VectorEdge Minimization::GetConformityPreviousGroupEdge(DualVectorInt& conformityGroupVectorPrevious, const DualVectorInt& conformityGroupVector, const VectorEdge& conformityGroupEdge)
{
	VectorEdge conformityPreviousGroupEdge;
	int size = m_stateCount * m_inputSize;
	VectorEdge outputState(size);

	for (size_t i = 0; i < size; ++i)
	{
		VectorEdge conformityGroupEdgeNext, conformityGroupAll;
		DualVectorInt conformityGroupVectorNext;

		if (i != 0)
		{
			VectorEdge groupOutputEdgeNext = GettingGroupOutputEdgeMealy(outputState);
			VectorEdge uniqueEdgeNext = GettingUniqueEdgeNext(groupOutputEdgeNext, conformityGroupVectorPrevious);
			conformityGroupEdgeNext = GettingConformityGroupEdgeNext(groupOutputEdgeNext, uniqueEdgeNext, conformityGroupVectorNext);
		}

		conformityGroupAll = i == 0 ? conformityGroupEdge : conformityGroupEdgeNext;

		if (m_automat == Automat::MEALY)
		{
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
		}
		else
		{
			for (int j = 0; j < m_outputCharacter.size(); ++j)
			{
				int indexRow = 0;
				int indexColumn = j;
				int unit = m_state[indexRow][indexColumn];
				for (int k = 0; k < m_inputSize; ++k)
				{
					auto it = std::find_if(conformityGroupAll.begin(), conformityGroupAll.end(), [&unit](const Edge& edge) { return edge.second == unit; });

					if (it != conformityGroupAll.end())
					{
						outputState[indexColumn] = std::make_pair((*it).second, (*it).first);
					}

					if (k < m_inputSize - 1)
					{
						++indexRow;
						unit = m_state[indexRow][j];
						indexColumn += m_stateCount;
					}
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

void Minimization::FillOutput(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge)
{
	int size = GetOutputStateSize();
	int sizeOutput = size * m_inputSize;
	m_outputState.resize(sizeOutput);

	for (int i = 0; i < size; ++i)
	{
		int indexInsert = i;
		int index = conformityGroupVectorPrevious[i][0];

		for (int j = 0; j < m_inputSize; ++j)
		{
			Edge inputEdgeFind = m_inputEdge[index];
			int unit = inputEdgeFind.first;

			auto it = std::find_if(conformityPreviousGroupEdge.begin(), conformityPreviousGroupEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

			if (it != conformityPreviousGroupEdge.end())
			{
				m_outputState[indexInsert] = std::make_pair((*it).first, inputEdgeFind.second);
			}

			if (j < m_inputSize - 1)
			{
				indexInsert += size;
				index += m_stateCount;
			}
		}
	}
}

void Minimization::FillOutputMoore(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge)
{
	int size = GetOutputStateSize();
	int sizeOutput = size * m_inputSize;
	m_outputStateMoore.resize(sizeOutput);

	for (int i = 0; i < size; ++i)
	{
		int indexInsert = i;
		int indexRow = 0;
		int indexColumn = conformityGroupVectorPrevious[i][0];
		m_outputCharacterMoore.push_back(m_outputCharacter[indexColumn]);

		for (int j = 0; j < m_inputSize; ++j)
		{
			int unit = m_state[indexRow][indexColumn];
			auto it = std::find_if(conformityPreviousGroupEdge.begin(), conformityPreviousGroupEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

			if (it != conformityPreviousGroupEdge.end())
			{
				m_outputStateMoore[indexInsert] = (*it).first;
			}

			if (j < m_inputSize - 1)
			{
				++indexRow;
				indexInsert += size;
			}
		}
	}
}

VectorEdge Minimization::MinimizationMealy()
{
	VectorEdge groupOutputEdge = GettingGroupOutputEdgeMealy(m_inputEdge);

	VectorEdge uniqueEdge = GettingUniqueMealy(groupOutputEdge, m_stateCount);

	VectorEdge conformityGroupEdge = GettingConformityGroupEdge(groupOutputEdge, uniqueEdge, m_conformityGroupVector);

	VectorEdge conformityPreviousGroupEdge = GetConformityPreviousGroupEdge(m_conformityGroupVectorPrevious, m_conformityGroupVector, conformityGroupEdge);

	FillOutput(m_conformityGroupVectorPrevious, conformityPreviousGroupEdge);

	return m_outputState;
}

VectorInt Minimization::MinimizationMoore()
{
	VectorInt uniqueItem = GettingUniqueMoore(m_outputCharacter);

	VectorEdge groupOutputEdge = GettingGroupOutputEdgeMoore(m_outputCharacter, uniqueItem, m_conformityGroupVector);

	VectorEdge conformityPreviousGroupEdge = GetConformityPreviousGroupEdge(m_conformityGroupVectorPrevious, m_conformityGroupVector, groupOutputEdge);

	FillOutputMoore(m_conformityGroupVectorPrevious, conformityPreviousGroupEdge);

	return m_outputStateMoore;
}