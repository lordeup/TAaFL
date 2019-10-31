#include "Automat.h"

Automat::Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const DualVectorSize_t& inputState)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_inputState(inputState)
{
}

void Automat::Determination()
{
	FillingColumns();
	CheckDetermination();
}

void Automat::FindUniqueCell(const VectorSize_t& cell)
{
	if (cell.size() > 1 && std::find(m_determinationState.begin(), m_determinationState.end(), cell) == m_determinationState.end())
	{
		m_determinationState.push_back(cell);
	}
}

void Automat::FillingColumns()
{
	m_outputState.resize(m_stateCount);

	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		m_outputState[i].resize(m_inputSize);
		DualVectorSize_t temporary;
		size_t index = i;

		for (const auto& column : m_outputState[i])
		{
			VectorSize_t numbersVector = m_inputState[index];
			temporary.push_back(numbersVector);
			FindUniqueCell(numbersVector);

			index += m_stateCount;
		}

		m_outputState[i] = temporary;
	}
}

void Automat::CheckDetermination()
{
	for (size_t i = 0; i < m_determinationState.size(); ++i)
	{
		DualVectorSize_t temporary(m_inputSize);

		for (const auto& column : m_determinationState[i])
		{
			DualVectorSize_t output = m_outputState[column];

			for (size_t j = 0; j < output.size(); ++j)
			{
				temporary[j].insert(temporary[j].end(), output[j].begin(), output[j].end());
			}
		}

		UniqueVector(temporary);

		m_outputState.push_back(temporary);
	}
}

void Automat::UniqueVector(DualVectorSize_t& columnVector)
{
	for (auto& column : columnVector)
	{
		std::sort(column.begin(), column.end());
		column.erase(std::unique(column.begin(), column.end()), column.end());
		FindUniqueCell(column);
	}
}

void Automat::GraphView() const
{
	VectorString vertexString;
	for (size_t i = 0; i < m_stateCount; ++i)
	{
		vertexString.push_back(std::to_string(i));
	}

	for (const auto& column : m_determinationState)
	{
		std::string str;
		for (const auto& cell : column)
		{
			str += std::to_string(cell);
		}
		vertexString.push_back(str);
	}

	std::ofstream outputGraph("outputGraph.dot");

	outputGraph << "digraph G {" << std::endl;

	for (const auto& vertex : vertexString)
	{
		outputGraph << vertex << ";" << std::endl;
	}

	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		size_t index = i;
		for (size_t j = 0; j < m_inputSize; ++j)
		{
			if (m_stateString[index].length())
			{
				outputGraph << vertexString[i] << "->" << m_stateString[index] << "  [label=x" << j << "];" << std::endl;
			}

			index += m_outputState.size();
		}
	}

	outputGraph << "}" << std::endl;
}

void Automat::PrintInfo()
{
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputState)
		{
			std::string str;
			for (const auto& cell : column[i])
			{
				m_output << "q" << cell;
				str += std::to_string(cell);
			}

			if (column[i].empty())
			{
				m_output << "-";
			}

			m_output << " ";
			m_stateString.push_back(str);
		}

		m_output << std::endl;
	}
}
