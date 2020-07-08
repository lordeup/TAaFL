#include "SyntacticalAnalyzer.h"

SyntacticalAnalyzer::SyntacticalAnalyzer(std::vector<GuideSetsData> guideSets, std::vector<std::string> headerSymbols, std::vector<LRData> lrData, std::list<std::string> sentence)
	: m_guideSets(guideSets)
	, m_headerSymbols(headerSymbols)
	, m_lrData(lrData)
	, m_sentence(sentence)
{
}

void SyntacticalAnalyzer::Run()
{
	m_currentLRData = m_lrData.front();
	m_stackLRData.push(m_currentLRData);

	while (!m_sentence.empty() || !m_stackLRData.empty())
	{
		std::string currentSentenceChar = m_sentence.empty() ? m_stackSentence.top() : m_sentence.front();
		Symbol symbol = GetSymbolByChInLRData(currentSentenceChar);

		if (symbol.state == StateSymbol::Shift || (currentSentenceChar == "e" && symbol.number - 1 == 0 && m_sentence.size() == 1))
		{
			m_sentence.pop_front();
			m_stackSentence.push(currentSentenceChar);
			m_logger.Log(currentSentenceChar + "\tadd to INPUT CHARACTERS stack");
		}

		if (symbol.state == StateSymbol::Shift)
		{
			m_currentLRData = m_lrData[symbol.number - 1];
			m_stackLRData.push(m_currentLRData);
			m_logger.Log(std::to_string(m_currentLRData.number) + "\tadd to STATE stack");
		}
		else if (symbol.state == StateSymbol::Convolution)
		{
			GuideSetsData guideSetsData = m_guideSets[symbol.number - 1];
			if (guideSetsData.terminals.size() == 1 && guideSetsData.terminals[0] == "e")
			{
			}
			else
			{
				for (size_t i = 0; i < guideSetsData.terminals.size(); i++)
				{
					if (!m_stackSentence.empty())
					{
						m_logger.Log(m_stackSentence.top() + "\tdeleted from INPUT CHARACTERS stack");
						m_stackSentence.pop();
					}
					if (!m_stackLRData.empty())
					{
						m_logger.Log(std::to_string(m_stackLRData.top().number) + "\tdeleted from STATE stack");
						m_stackLRData.pop();
					}
				}
			}

			m_sentence.push_front(guideSetsData.nonterminal);

			m_logger.Log(guideSetsData.nonterminal + "\tadd to start position in SENTENCE");

			if (!m_stackLRData.empty())
			{
				m_currentLRData = m_stackLRData.top();
			}
		}
		else if (symbol.state == StateSymbol::Ok)
		{
			PrintSentence();
			PrintStackLRData();
			PrintStackSentence();
			break;
		}
		else if (symbol.state == StateSymbol::None)
		{
			m_logger.Log("State none for header character '" + currentSentenceChar + "' in row number " + std::to_string(m_currentLRData.number));
			m_logger.Print();
			throw std::invalid_argument("State none for header character '" + currentSentenceChar + "' in row number " + std::to_string(m_currentLRData.number));
		}

		LogStackLRDataInfo();
		LogStackSentenceInfo();
		LogSentenceInfo();
		m_logger.Log("--------------------------------------------------------------------------------------");
	}
	m_logger.Print();
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

size_t SyntacticalAnalyzer::GetIndexInHeaderSymbols(std::string headerSymbol)
{
	size_t index = -1;
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

void SyntacticalAnalyzer::LogStackLRDataInfo()
{
	std::stack<LRData> copyLRDataStack(m_stackLRData);
	std::string str;

	while (!copyLRDataStack.empty())
	{
		str.append(std::to_string(copyLRDataStack.top().number) + " ");
		copyLRDataStack.pop();
	}
	m_logger.Log("STATE stack items: " + str);
}

void SyntacticalAnalyzer::LogStackSentenceInfo()
{
	std::stack<std::string> copySentenceStack(m_stackSentence);
	std::string str;

	while (!copySentenceStack.empty())
	{
		str.append(copySentenceStack.top() + " ");
		copySentenceStack.pop();
	}
	m_logger.Log("INPUT CHARACTERS stack items: " + str);
}

void SyntacticalAnalyzer::LogSentenceInfo()
{
	std::string str;
	for (auto ch : m_sentence)
	{
		str.append(ch + " ");
	}
	m_logger.Log("SENTENCE items: " + str);
}

void SyntacticalAnalyzer::PrintStackLRData()
{
	if (m_stackLRData.empty())
	{
		std::cout << "The STATE stack is empty = OK" << std::endl;
	}
	else
	{
		std::cout << "The STATE stack is not empty, it contains: ";
		while (!m_stackLRData.empty())
		{
			std::cout << m_stackLRData.top().ch << ", ";
			m_stackLRData.pop();
		}
		std::cout << std::endl;
	}
}

void SyntacticalAnalyzer::PrintStackSentence()
{
	if (m_stackSentence.empty())
	{
		std::cout << "INPUT CHARACTERS stack is empty = OK" << std::endl;
	}
	else
	{
		std::cout << "The stack of INPUT CHARACTERS is not empty, it contains: ";
		while (!m_stackSentence.empty())
		{
			std::cout << m_stackSentence.top() << ", ";
			m_stackSentence.pop();
		}
		std::cout << std::endl;
	}
}

void SyntacticalAnalyzer::PrintSentence()
{
	if (m_sentence.empty())
	{
		std::cout << "The INPUT SENTENCE is empty = OK" << std::endl;
	}
	else
	{
		std::cout << "The INPUT SENTENCE is not empty, it contains: ";
		for (auto ch : m_sentence)
		{
			std::cout << ch << ", ";
		}
		std::cout << std::endl;
	}
}