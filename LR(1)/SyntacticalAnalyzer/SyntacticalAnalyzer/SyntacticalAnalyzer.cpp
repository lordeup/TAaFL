#include "SyntacticalAnalyzer.h"

SyntacticalAnalyzer::SyntacticalAnalyzer(std::vector<std::string> headerSymbols, std::vector<LRData> lrData, std::list<std::string> sentence)
	: m_headerSymbols(headerSymbols)
	, m_lrData(lrData)
	, m_sentence(sentence)
{
}

void SyntacticalAnalyzer::Run()
{
	m_currentLRData = m_lrData.front();
	std::stack<LRData> m_stackLRData;
	m_stackLRData.push(m_currentLRData);

	bool isCurrentStateConvolution = false;

	while (!m_sentence.empty())
	{
		std::string currentHeaderSymbol = m_sentence.front();

		if (!isCurrentStateConvolution)
		{
			m_stackSentence.push(currentHeaderSymbol);
			m_sentence.pop_front();
		}

		Symbol symbol = GetSymbolByChInLRData(currentHeaderSymbol);

		if (symbol.state == State::Shift)
		{
			m_currentLRData = m_lrData[symbol.number - 1];
			m_stackLRData.push(m_currentLRData);

			std::string nextHeaderSymbol = m_sentence.front();
			Symbol nextSymbol = GetSymbolByChInLRData(nextHeaderSymbol);
			if (nextSymbol.state == State::Convolution)
			{
				isCurrentStateConvolution = true;
			}
		}
		else if (symbol.state == State::Convolution)
		{
			m_currentLRData = m_lrData[symbol.number - 2];

			for (size_t i = 0; i < m_currentLRData.size; i++)
			{
				if (!m_stackLRData.empty())
				{
					m_stackLRData.pop();
				}
				if (!m_stackSentence.empty())
				{
					m_stackSentence.pop();
				}
			}
			m_sentence.push_front(m_currentLRData.rule);
			isCurrentStateConvolution = false;

			if (!m_stackLRData.empty())
			{
				m_currentLRData = m_stackLRData.top();
			}
		}
		else if (symbol.state == State::Ok)
		{
			if (m_stackLRData.empty())
			{
				std::cout << "m_stackLRData is empty = OK" << std::endl;
			}
			if (m_stackSentence.empty())
			{
				std::cout << "m_stackSentence is empty = OK" << std::endl;
			}
		}
		else if (symbol.state == State::None)
		{
			throw std::exception("state none");
		}
	}
}

LRData SyntacticalAnalyzer::GetLRDataByCh(std::string currentChar)
{
	auto it = std::find_if(m_lrData.begin(), m_lrData.end(), [&](const LRData& data) {
		return data.ch == currentChar;
	});

	if (it == m_lrData.end())
	{
		throw std::invalid_argument("Error. Wrong character: " + currentChar);
	}

	return *it;
}

Symbol SyntacticalAnalyzer::GetSymbolByChInLRData(std::string headerSymbol)
{
	size_t index = GetIndexInHeaderSymbols(headerSymbol);
	return m_currentLRData.symbols[index];
}

int SyntacticalAnalyzer::GetIndexInHeaderSymbols(std::string headerSymbol)
{
	int index = -1;
	for (size_t i = 0; i < m_headerSymbols.size(); i++)
	{
		if (m_headerSymbols[i] == headerSymbol)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		throw std::invalid_argument("Error. Wrong character in header symbol: " + headerSymbol);
	}

	return index;
}