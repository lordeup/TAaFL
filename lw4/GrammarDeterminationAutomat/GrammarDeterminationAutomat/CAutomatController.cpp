#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
}

void CAutomatController::ProcessingCommand()
{
	size_t inputSize, stateCount, rulesCount;

	m_input >> inputSize >> stateCount >> rulesCount;

	try
	{
		FillingData(inputSize, stateCount, rulesCount);
		DefinitionGrammar();

		Automat automat(m_output, inputSize, stateCount, rulesCount, m_stateMap, m_grammar);
		automat.Determination();
		automat.PrintInfo();
		automat.GraphView();
	}
	catch (const std::exception& error)
	{
		m_output << error.what();
	}
}

void CAutomatController::DefinitionGrammar()
{
	if (!m_stateMap.empty())
	{
		VectorPairString vec = (*m_stateMap.begin()).second;
		if (!vec.empty())
		{
			std::string str = vec[0].first;
			m_grammar = str[0] >= 65 && str[0] <= 90 ? Grammar::LEFT : Grammar::RIGHT;
		}
	}
}

std::string CAutomatController::GetConvertToString(const char ch) const
{
	std::string str;
	str = (const char)ch;
	return str;
}

void CAutomatController::FillingData(const size_t inputSize, const size_t stateCount, const size_t rulesCount)
{
	char ch, ch1, ch2;

	for (size_t i = 0; i < stateCount; ++i)
	{
		bool isNext = true;
		m_input >> ch;

		VectorPairString temporary;

		while (isNext)
		{
			ch1 = ch2 = '\0';
			m_input >> ch1 >> ch2;
			temporary.push_back(std::make_pair(GetConvertToString(ch1), GetConvertToString(ch2)));
			if (m_input.get() == '\n' || m_input.eof())
			{
				isNext = false;
			}
		}

		m_stateMap.emplace(std::make_pair(i, GetConvertToString(ch)), temporary);
	}
}