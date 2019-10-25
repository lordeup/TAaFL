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

			if (numbersVector.size() > 1)
			{
				m_determinationState.push_back(numbersVector);
			}

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
	}

	for (const auto& column : columnVector)
	{
		if (column.size() > 1 && std::find(m_determinationState.begin(), m_determinationState.end(), column) == m_determinationState.end())
		{
			m_determinationState.push_back(column);
		}
	}
}

void Automat::GraphView() const
{
	Visualization visualization(m_outputTest, m_outputState.size());
	visualization.GraphView();
}

size_t count_numbers(size_t num)
{
	size_t count = 0;

	while (num != 0)
	{
		count++;
		num /= 10;
	}

	return count;
}

size_t combine(size_t a, size_t b)
{
	size_t times = 1;

	while (times <= b)
	{
		times *= 10;
	}

	return a * times + b;
}

void Automat::Test(std::string str)
{
	size_t number, numberWithZero;

	try
	{
		number = std::stoi(str);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument(ERROR_WRONG_DATA);
	}

	//if (str.length() != count_numbers(number))
	//{
	//	numberWithZero = combine(number, 0);
	//}

	m_outputTest.push_back(number);
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
				m_output << SYMBOL_Q << cell;
				str += std::to_string(cell);
			}

			if (column[i].empty())
			{
				m_output << SKIP;
			}

			m_output << SPASE;
			Test(str);
		}

		m_output << std::endl;
	}
}
