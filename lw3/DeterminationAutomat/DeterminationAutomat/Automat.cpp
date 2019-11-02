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
	bool isCheckSkipState = false;
	for (size_t i = 0; i < m_stateCount; ++i)
	{
		DualVectorSize_t temporary;
		size_t index = i;

		for (size_t j = 0; j < m_inputSize; ++j)
		{
			temporary.push_back(m_inputState[index]);
			if (!m_inputState[index].empty() && !isCheckSkipState)
			{
				m_determinationState.push_back({ i });
				isCheckSkipState = true;
			}
			index += m_stateCount;
		}

		m_inputStateColumn.push_back(temporary);
	}

	for (size_t i = 0; i < m_determinationState.size(); ++i)
	{
		DualVectorSize_t temporary(m_inputSize);

		for (const auto& column : m_determinationState[i])
		{
			DualVectorSize_t output = m_inputStateColumn[column];

			for (size_t j = 0; j < output.size(); ++j)
			{
				temporary[j].insert(temporary[j].end(), output[j].begin(), output[j].end());
			}
		}

		if (!m_determinationState[i].empty())
		{
			UniqueVector(temporary);
			m_outputState.push_back(temporary);
		}
	}
}

void Automat::UniqueVector(DualVectorSize_t& columnVector)
{
	for (auto& column : columnVector)
	{
		std::sort(column.begin(), column.end());
		column.erase(std::unique(column.begin(), column.end()), column.end());

		if (!column.empty() && std::find(m_determinationState.begin(), m_determinationState.end(), column) == m_determinationState.end())
		{
			m_determinationState.push_back(column);
		}
	}
}

VectorString Automat::ConvertStateString()
{
	VectorString stateString;
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputState)
		{
			std::string str;
			for (const auto& cell : column[i])
			{
				str += std::to_string(cell);
			}
			stateString.push_back(str);
		}
	}
	return stateString;
}

VectorString Automat::ConvertVertexString()
{
	VectorString vertexString;
	for (const auto& column : m_determinationState)
	{
		if (!column.empty())
		{
			std::string str;
			for (const auto& cell : column)
			{
				str += std::to_string(cell);
			}
			vertexString.push_back(str);
		}
	}
	return vertexString;
}

void Automat::GraphView()
{
	VectorString stateString = ConvertStateString();

	VectorString vertexString = ConvertVertexString();

	std::ofstream outputGraph("outputGraph.dot");

	outputGraph << "digraph G {" << std::endl;

	for (const auto& vertex : vertexString)
	{
		outputGraph << vertex << ";" << std::endl;
	}

	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		size_t index = i;
		for (size_t x = 0; x < m_inputSize; ++x)
		{
			if (!stateString[index].empty())
			{
				outputGraph << vertexString[i] << "->" << stateString[index] << "  [label=x" << x << "];" << std::endl;
			}

			index += m_outputState.size();
		}
	}

	outputGraph << "}" << std::endl;
}

void Automat::PrintInfo() const
{
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputState)
		{
			for (const auto& cell : column[i])
			{
				m_output << "q" << cell;
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
