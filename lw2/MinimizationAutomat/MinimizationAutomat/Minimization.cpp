#include "Minimization.h"

Minimization::Minimization(const size_t inputSize, const size_t stateCount, const VectorEdge& inputEdge, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputEdge(inputEdge)
	, m_automat(automat)
{
}

Minimization::Minimization(const size_t inputSize, const size_t stateCount, const VectorSize_t& outputCharacter, const DualVectorSize_t& state, const Automat automat)
	: m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_outputCharacter(outputCharacter)
	, m_state(state)
	, m_automat(automat)
{
}

size_t Minimization::GetOutputStateSize() const
{
	return m_groupPrevious.size();
}

VectorSize_t Minimization::GetOutputCharacterMoore() const
{
	return m_outputCharacterMoore;
}

DualVectorSize_t Minimization::GettingGroup(const VectorEdge& inputEdge)
{
	DualVectorSize_t output(m_stateCount);
	for (size_t i = 0; i < output.size(); ++i)
	{
		output[i].resize(m_inputSize);
		VectorSize_t temporary;
		size_t index = i;
		for (size_t j = 0; j < output[i].size(); ++j)
		{
			temporary.push_back(inputEdge[index].second);
			index += m_stateCount;
		}
		output[i] = temporary;
	}

	return output;
}

VectorEdge Minimization::GettingGroupMoore()
{
	VectorSize_t unique;
	std::set<size_t> setUnique(m_outputCharacter.begin(), m_outputCharacter.end());

	unique.insert(unique.end(), setUnique.begin(), setUnique.end());

	VectorEdge groupEdge(m_stateCount);
	m_groupVector.resize(unique.size());

	for (size_t i = 0; i < unique.size(); ++i)
	{
		for (size_t j = 0; j < m_outputCharacter.size(); ++j)
		{
			if (unique[i] == m_outputCharacter[j])
			{
				groupEdge[j] = std::make_pair(i, j);
				m_groupVector[i].push_back(j);
			}
		}
	}

	return groupEdge;
}

VectorEdge Minimization::GettingGroupEdge(const DualVectorSize_t& group)
{
	DualVectorSize_t unique(group.size());

	std::copy(group.begin(), group.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());

	VectorEdge groupEdge(m_stateCount);
	m_groupVector.resize(unique.size());

	for (size_t i = 0; i < unique.size(); ++i)
	{
		for (size_t j = 0; j < group.size(); ++j)
		{
			if (unique[i] == group[j])
			{
				groupEdge[j] = std::make_pair(i, j);
				m_groupVector[i].push_back(j);
			}
		}
	}

	return groupEdge;
}

VectorEdge Minimization::GettingGroupNext(const VectorEdge& outputState, DualVectorSize_t& groupPrevious, DualVectorSize_t& groupVector)
{
	DualVectorSize_t group = GettingGroup(outputState);
	DualVectorSize_t unique;

	for (auto it = groupPrevious.begin(); it != groupPrevious.end(); ++it)
	{
		std::set<VectorSize_t> setUnique;

		for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			setUnique.insert(group[*it2]);
		}

		unique.insert(unique.end(), setUnique.begin(), setUnique.end());
	}

	VectorEdge groupEdge(m_stateCount);
	groupVector.resize(unique.size());

	for (size_t i = 0, index = 0, sum = 0; i < unique.size(); ++i, ++index)
	{
		for (size_t j = 0; j < m_groupPrevious.size(); ++j)
		{
			size_t size = m_groupPrevious[index].size();
			for (size_t k = 0; k < size; ++k)
			{
				size_t unit = m_groupPrevious[index][k];

				if (unique[i] == group[unit])
				{
					groupEdge[unit] = std::make_pair(i, unit);
					groupVector[i].push_back(unit);
				}
			}

			sum += groupVector[i].size();
			sum == size ? sum = 0 : --index;

			break;
		}
	}

	std::sort(groupVector.begin(), groupVector.end());

	return groupEdge;
}

VectorEdge Minimization::GettingGroupPreviousEdge(DualVectorSize_t& groupPrevious, const DualVectorSize_t& groupVector, const VectorEdge& groupEdge)
{
	VectorEdge groupPreviousEdge;
	VectorEdge outputState(m_stateCount * m_inputSize);

	for (size_t i = 0; i < outputState.size(); ++i)
	{
		VectorEdge groupNextEdge, groupResultEdge;
		DualVectorSize_t groupNextVector;

		if (i != 0)
		{
			groupNextEdge = GettingGroupNext(outputState, groupPrevious, groupNextVector);
		}

		groupResultEdge = i == 0 ? groupEdge : groupNextEdge;

		if (m_automat == Automat::MEALY)
		{
			for (size_t j = 0; j < m_stateCount; ++j)
			{
				size_t indexEdge = j;
				size_t unit = m_inputEdge[indexEdge].first;

				for (size_t k = 0; k < m_inputSize; ++k)
				{
					auto it = std::find_if(groupResultEdge.begin(), groupResultEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

					if (it != groupResultEdge.end())
					{
						outputState[indexEdge] = std::make_pair(0, (*it).first);
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
			for (size_t j = 0; j < m_outputCharacter.size(); ++j)
			{
				size_t indexRow = 0;
				size_t indexColumn = j;
				size_t unit = m_state[indexRow][indexColumn];
				for (size_t k = 0; k < m_inputSize; ++k)
				{
					auto it = std::find_if(groupResultEdge.begin(), groupResultEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

					if (it != groupResultEdge.end())
					{
						outputState[indexColumn] = std::make_pair(0, (*it).first);
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

		if (groupPreviousEdge == groupResultEdge)
		{
			break;
		}

		groupPreviousEdge = groupResultEdge;
		groupPrevious = i == 0 ? groupVector : groupNextVector;
	}

	return groupPreviousEdge;
}

void Minimization::FillOutputMealy(const VectorEdge& groupPreviousEdge)
{
	size_t size = GetOutputStateSize();
	m_outputState.resize(size * m_inputSize);

	for (size_t i = 0; i < size; ++i)
	{
		size_t indexInsert = i;
		size_t index = m_groupPrevious[i][0];

		for (size_t j = 0; j < m_inputSize; ++j)
		{
			Edge inputFindEdge = m_inputEdge[index];
			size_t unit = inputFindEdge.first;

			auto it = std::find_if(groupPreviousEdge.begin(), groupPreviousEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

			if (it != groupPreviousEdge.end())
			{
				m_outputState[indexInsert] = std::make_pair((*it).first, inputFindEdge.second);
			}

			if (j < m_inputSize - 1)
			{
				indexInsert += size;
				index += m_stateCount;
			}
		}
	}
}

void Minimization::FillOutputMoore(const VectorEdge& groupPreviousEdge)
{
	size_t size = GetOutputStateSize();
	m_outputStateMoore.resize(size * m_inputSize);

	for (size_t i = 0; i < size; ++i)
	{
		size_t indexInsert = i;
		size_t indexRow = 0;
		size_t indexColumn = m_groupPrevious[i][0];
		m_outputCharacterMoore.push_back(m_outputCharacter[indexColumn]);

		for (size_t j = 0; j < m_inputSize; ++j)
		{
			size_t unit = m_state[indexRow][indexColumn];
			auto it = std::find_if(groupPreviousEdge.begin(), groupPreviousEdge.end(), [&unit](const Edge& edge) { return edge.second == unit; });

			if (it != groupPreviousEdge.end())
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
	DualVectorSize_t groupVector = GettingGroup(m_inputEdge);
	VectorEdge groupEdge = GettingGroupEdge(groupVector);
	VectorEdge groupPreviousEdge = GettingGroupPreviousEdge(m_groupPrevious, m_groupVector, groupEdge);

	FillOutputMealy(groupPreviousEdge);

	return m_outputState;
}

VectorSize_t Minimization::MinimizationMoore()
{
	VectorEdge groupEdge = GettingGroupMoore();
	VectorEdge groupPreviousEdge = GettingGroupPreviousEdge(m_groupPrevious, m_groupVector, groupEdge);

	FillOutputMoore(groupPreviousEdge);

	return m_outputStateMoore;
}