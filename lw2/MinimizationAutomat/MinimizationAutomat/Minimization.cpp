#include "Minimization.h"

Minimization::Minimization(const size_t inputSize, const size_t stateCount, const VectorEdge& inputEdge)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
{
}

Minimization::Minimization(const size_t inputSize, const size_t stateCount, const VectorSize_t& outputCharacter, const DualVectorSize_t& state)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
{
}

DualVectorSize_t Minimization::GetTest(const VectorEdge& inputEdge)
{
	DualVectorSize_t output(m_stateCount);
	for (size_t i = 0; i < m_stateCount; ++i)
	{
		output[i].resize(m_inputSize);
		VectorSize_t temp;
		size_t index = i;
		for (size_t j = 0; j < m_inputSize; ++j)
		{
			temp.push_back(inputEdge[index].second);
			index += m_stateCount;
		}
		output[i] = temp;
	}

	return output;
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

VectorEdge Minimization::GettingGroupOutputEdgeMoore(const VectorSize_t& outputCharacter, const VectorSize_t& uniqueItem, DualVectorSize_t& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueItem.size());

	for (size_t i = 0; i < uniqueItem.size(); ++i)
	{
		for (size_t j = 0; j < outputCharacter.size(); ++j)
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

size_t Minimization::GetOutputStateSize() const
{
	return m_conformityGroupVectorPrevious.size();
}

VectorSize_t Minimization::GetOutputCharacterMoore() const
{
	return m_outputCharacterMoore;
}

DualVectorSize_t Minimization::GettingUniqueMealy(const DualVectorSize_t& groupOutputEdge)
{
	DualVectorSize_t uniqueEdge(groupOutputEdge.size());

	std::copy(groupOutputEdge.begin(), groupOutputEdge.end(), uniqueEdge.begin());
	std::sort(uniqueEdge.begin(), uniqueEdge.end());
	uniqueEdge.erase(std::unique(uniqueEdge.begin(), uniqueEdge.end()), uniqueEdge.end());

	return uniqueEdge;
}

VectorSize_t Minimization::GettingUniqueMoore(const VectorSize_t& groupOutputEdge)
{
	VectorSize_t uniqueEdge;
	std::set<size_t> setUnique;

	for (const auto& group : groupOutputEdge)
	{
		setUnique.insert(group);
	}

	uniqueEdge.reserve(uniqueEdge.size() + setUnique.size());
	uniqueEdge.insert(uniqueEdge.end(), setUnique.begin(), setUnique.end());

	return uniqueEdge;
}

VectorEdge Minimization::GettingUniqueEdgeNext(const VectorEdge& groupOutputEdge, const DualVectorSize_t& conformityGroupVector)
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

DualVectorSize_t Minimization::GetNextUnicTest(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& conformityGroupVector)
{
	DualVectorSize_t uniqueEdge;

	for (auto it = conformityGroupVector.begin(); it != conformityGroupVector.end(); ++it)
	{
		std::set<VectorSize_t> setUnique;

		for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			setUnique.insert(groupOutputEdge[*it2]);
		}

		uniqueEdge.reserve(uniqueEdge.size() + setUnique.size());
		uniqueEdge.insert(uniqueEdge.end(), setUnique.begin(), setUnique.end());
	}

	return uniqueEdge;
}

VectorEdge Minimization::GettingConformityGroupEdge(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& uniqueEdge)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	m_conformityGroupVector.resize(uniqueEdge.size());

	for (size_t i = 0; i < uniqueEdge.size(); ++i)
	{
		for (size_t j = 0; j < groupOutputEdge.size(); ++j)
		{
			if (uniqueEdge[i] == groupOutputEdge[j])
			{
				conformityGroupEdge[j] = std::make_pair(i, j);
				m_conformityGroupVector[i].push_back(j);
			}
		}
	}

	return conformityGroupEdge;
}

VectorEdge Minimization::GeConfGrouptTest(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& uniqueEdge, DualVectorSize_t& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueEdge.size());

	for (size_t i = 0, index = 0, sum = 0; i < uniqueEdge.size(); ++i, ++index)
	{
		for (size_t j = 0; j < m_conformityGroupVectorPrevious.size(); ++j)
		{
			size_t size = m_conformityGroupVectorPrevious[index].size();
			for (size_t k = 0; k < size; ++k)
			{
				size_t unit = m_conformityGroupVectorPrevious[index][k];

				if (uniqueEdge[i] == groupOutputEdge[unit])
				{
					conformityGroupEdge[unit] = std::make_pair(i, unit);
					conformityGroupVector[i].push_back(unit);
				}
			}

			sum += conformityGroupVector[i].size();
			sum == size ? sum = 0 : --index;

			break;
		}
	}

	//std::sort(conformityGroupVector.begin(), conformityGroupVector.end());

	std::sort(conformityGroupVector.begin(), conformityGroupVector.end(), [](const auto& a, const auto& b) {
		return *(a.begin()) < *(b.begin());
	});

	return conformityGroupEdge;
}

VectorEdge Minimization::GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorSize_t& conformityGroupVector)
{
	VectorEdge conformityGroupEdge(m_stateCount);
	conformityGroupVector.resize(uniqueEdge.size());

	for (size_t i = 0, index = 0, sum = 0; i < uniqueEdge.size(); ++i, ++index)
	{
		for (size_t j = 0; j < m_conformityGroupVectorPrevious.size(); ++j)
		{
			size_t size = m_conformityGroupVectorPrevious[index].size();
			for (size_t k = 0; k < size; ++k)
			{
				size_t unit = m_conformityGroupVectorPrevious[index][k];

				if (uniqueEdge[i] == groupOutputEdge[unit])
				{
					conformityGroupEdge[unit] = std::make_pair(i, unit);
					conformityGroupVector[i].push_back(unit);
				}
			}

			sum += conformityGroupVector[i].size();
			sum == size ? sum = 0 : --index;

			break;
		}
	}

	std::sort(conformityGroupVector.begin(), conformityGroupVector.end(), [](const auto& a, const auto& b) {
		return *(a.begin()) < *(b.begin());
	});

	return conformityGroupEdge;
}

VectorEdge Minimization::GetPrevTest(DualVectorSize_t& conformityGroupVectorPrevious, const DualVectorSize_t& conformityGroupVector, const VectorEdge& conformityGroupEdge)
{
	VectorEdge conformityPreviousGroupEdge;
	size_t size = m_stateCount * m_inputSize;
	VectorEdge outputState(size);

	for (size_t i = 0; i < size; ++i)
	{
		VectorEdge conformityGroupEdgeNext, conformityGroupAll;
		DualVectorSize_t conformityGroupVectorNext;

		if (i != 0)
		{
			DualVectorSize_t groupOutputEdgeNext = GetTest(outputState);
			DualVectorSize_t uniqueEdgeNext = GetNextUnicTest(groupOutputEdgeNext, conformityGroupVectorPrevious);
			conformityGroupEdgeNext = GeConfGrouptTest(groupOutputEdgeNext, uniqueEdgeNext, conformityGroupVectorNext);
		}

		conformityGroupAll = i == 0 ? conformityGroupEdge : conformityGroupEdgeNext;

		for (size_t j = 0; j < m_stateCount; ++j)
		{
			size_t indexEdge = j;
			size_t unit = m_inputEdge[indexEdge].first;

			for (size_t k = 0; k < m_inputSize; ++k)
			{
				auto it = std::find_if(conformityGroupAll.begin(), conformityGroupAll.end(), [&unit](const Edge& edge) { return edge.second == unit; });

				if (it != conformityGroupAll.end())
				{
					outputState[indexEdge] = std::make_pair(11, (*it).first);
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

VectorEdge Minimization::GetConformityPreviousGroupEdge(DualVectorSize_t& conformityGroupVectorPrevious, const DualVectorSize_t& conformityGroupVector, const VectorEdge& conformityGroupEdge)
{
	VectorEdge conformityPreviousGroupEdge;
	VectorEdge outputState(m_stateCount * m_inputSize);

	for (size_t i = 0; i < outputState.size(); ++i)
	{
		VectorEdge conformityGroupEdgeNext, conformityGroupAll;
		DualVectorSize_t conformityGroupVectorNext;

		if (i != 0)
		{
			VectorEdge groupOutputEdgeNext = GettingGroupOutputEdgeMealy(outputState);
			VectorEdge uniqueEdgeNext = GettingUniqueEdgeNext(groupOutputEdgeNext, conformityGroupVectorPrevious);
			conformityGroupEdgeNext = GettingConformityGroupEdgeNext(groupOutputEdgeNext, uniqueEdgeNext, conformityGroupVectorNext);
		}

		conformityGroupAll = i == 0 ? conformityGroupEdge : conformityGroupEdgeNext;

		for (size_t j = 0; j < m_outputCharacter.size(); ++j)
		{
			size_t indexRow = 0;
			size_t indexColumn = j;
			size_t unit = m_state[indexRow][indexColumn];
			for (size_t k = 0; k < m_inputSize; ++k)
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

		if (conformityPreviousGroupEdge == conformityGroupAll)
		{
			break;
		}

		conformityPreviousGroupEdge = conformityGroupAll;
		conformityGroupVectorPrevious = i == 0 ? conformityGroupVector : conformityGroupVectorNext;
	}

	return conformityPreviousGroupEdge;
}

void Minimization::FillOutput(const DualVectorSize_t& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge)
{
	size_t size = GetOutputStateSize();
	m_outputState.resize(size * m_inputSize);

	for (size_t i = 0; i < size; ++i)
	{
		size_t indexInsert = i;
		size_t index = conformityGroupVectorPrevious[i][0];

		for (size_t j = 0; j < m_inputSize; ++j)
		{
			Edge inputEdgeFind = m_inputEdge[index];
			size_t unit = inputEdgeFind.first;

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

void Minimization::FillOutputMoore(const DualVectorSize_t& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge)
{
	size_t size = GetOutputStateSize();
	m_outputStateMoore.resize(size * m_inputSize);

	for (size_t i = 0; i < size; ++i)
	{
		size_t indexInsert = i;
		size_t indexRow = 0;
		size_t indexColumn = conformityGroupVectorPrevious[i][0];
		m_outputCharacterMoore.push_back(m_outputCharacter[indexColumn]);

		for (size_t j = 0; j < m_inputSize; ++j)
		{
			size_t unit = m_state[indexRow][indexColumn];
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
	DualVectorSize_t group = GetTest(m_inputEdge);

	DualVectorSize_t unic = GettingUniqueMealy(group);

	VectorEdge conformityGroupEdge = GettingConformityGroupEdge(group, unic);

	VectorEdge conformityPreviousGroupEdge = GetPrevTest(m_conformityGroupVectorPrevious, m_conformityGroupVector, conformityGroupEdge);

	FillOutput(m_conformityGroupVectorPrevious, conformityPreviousGroupEdge);

	return m_outputState;
}

VectorSize_t Minimization::MinimizationMoore()
{
	VectorSize_t uniqueItem = GettingUniqueMoore(m_outputCharacter);

	VectorEdge groupOutputEdge = GettingGroupOutputEdgeMoore(m_outputCharacter, uniqueItem, m_conformityGroupVector);

	VectorEdge conformityPreviousGroupEdge = GetConformityPreviousGroupEdge(m_conformityGroupVectorPrevious, m_conformityGroupVector, groupOutputEdge);

	FillOutputMoore(m_conformityGroupVectorPrevious, conformityPreviousGroupEdge);

	return m_outputStateMoore;
}