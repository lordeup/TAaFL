#include "Automat.h"

Automat::Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const size_t rulesCount, const MapEdge& stateMap, const Grammar grammar)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_rulesCount(rulesCount)
	, m_stateMap(stateMap)
	, m_grammar(grammar)
{
}

void Automat::AddDetermination()
{
	for (auto it = m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		VectorPairString vec = (*it).second;
		VectorString temporaryNoTerm;

		DualVectorPairString temporary(m_inputSize);
		m_term.push_back((*it).first.second);

		for (size_t j = 0; j < vec.size(); ++j)
		{
			temporaryNoTerm.push_back(vec[j].first);
			temporary[j].push_back(vec[j]);
		}
		UniqueNoTerm(temporaryNoTerm);
		m_inputStateChar.push_back(temporary);
	}
}

void Automat::UniqueNoTerm(VectorString& columnVector)
{
	for (auto& column : columnVector)
	{
		std::sort(column.begin(), column.end());
		column.erase(std::unique(column.begin(), column.end()), column.end());

		if (std::find(m_noTerm.begin(), m_noTerm.end(), column) == m_noTerm.end())
		{
			m_noTerm.push_back(column);
		}
	}
}

void Automat::UniqueVector(DualVectorString& columnVector)
{
	for (auto& column : columnVector)
	{
		if (!column.empty())
		{
			std::sort(column.begin(), column.end());
			column.erase(std::unique(column.begin(), column.end()), column.end());

			if (std::find(m_determinationStateString.begin(), m_determinationStateString.end(), column) == m_determinationStateString.end())
			{
				m_determinationStateString.push_back(column);
			}
		}
	}
}

void Automat::DeterminationRightGrammar()
{
	m_determinationStateString.push_back({ m_term[0] });

	for (size_t i = 0; i < m_determinationStateString.size(); ++i)
	{
		DualVectorString temporary(m_inputSize);

		for (const auto& column : m_determinationStateString[i])
		{
			auto it = std::find(m_term.begin(), m_term.end(), column);

			if (it != m_term.end())
			{
				size_t indexFind = std::distance(m_term.begin(), it);

				DualVectorPairString output = m_inputStateChar[indexFind];

				for (auto itt = output.begin(); itt != output.end(); ++itt)
				{
					if (!(*itt).empty())
					{
						auto element = (*itt)[0];
						auto itFind = std::find(m_noTerm.begin(), m_noTerm.end(), element.first);

						if (itFind != m_noTerm.end())
						{
							size_t indexFindNoTerm = std::distance(m_noTerm.begin(), itFind);
							std::string str = element.second[0] ? element.second : "F";
							temporary[indexFindNoTerm].push_back(str);
						}
					}
				}
			}
		}

		UniqueVector(temporary);
		m_outputStateString.push_back(temporary);
	}
}

void Automat::DeterminationLeftGrammar()
{
}

void Automat::Determination()
{
	if (m_grammar == Grammar::RIGHT)
	{
		AddDetermination();
		DeterminationRightGrammar();
	}
	else
	{
		DeterminationLeftGrammar();
	}
}

VectorString Automat::ConvertVertexString()
{
	VectorString vertexString;
	for (const auto& column : m_determinationStateString)
	{
		std::string str;
		for (const auto& cell : column)
		{
			str += cell;
		}
		vertexString.push_back(str);
	}
	return vertexString;
}

VectorPairString Automat::ConvertStateString()
{
	VectorPairString stateString;
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputStateString)
		{
			std::string str1, str2;
			for (const auto& cell : column[i])
			{
				str1 += cell;
				str2 = m_noTerm[i] == "#" ? "End" : m_noTerm[i];
			}
			stateString.push_back(std::make_pair(str1, str2));
		}
	}
	return stateString;
}

void Automat::GraphView()
{
	VectorPairString stateString = ConvertStateString();

	VectorString vertexString = ConvertVertexString();

	std::ofstream outputGraph("outputGraph.dot");

	outputGraph << "digraph G {" << std::endl;

	for (const auto& vertex : vertexString)
	{
		outputGraph << vertex << ";" << std::endl;
	}

	for (size_t i = 0; i < m_outputStateString.size(); ++i)
	{
		size_t index = i;
		for (size_t j = 0; j < m_inputSize; ++j)
		{
			if (!stateString[index].second.empty())
			{
				outputGraph << vertexString[i] << "->" << stateString[index].first << " [label=" << stateString[index].second << "];" << std::endl;
			}

			index += m_outputStateString.size();
		}
	}

	outputGraph << "}" << std::endl;
}

void Automat::PrintInfo() const
{
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputStateString)
		{
			for (const auto& cell : column[i])
			{
				m_output << cell;
			}

			if (column[i].empty())
			{
				m_output << "-";
			}

			m_output << " ";
		}

		m_output << std::endl;
	}
}
