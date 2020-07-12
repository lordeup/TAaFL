#include "SyntacticalAnalyzer.h"

SyntacticalAnalyzer::SyntacticalAnalyzer(std::vector<GuideSetsData> guideSets, std::vector<std::string> headerSymbols, std::vector<LRData> lrData, std::list<Token> sentence, Logger& logger)
	: m_guideSets(guideSets)
	, m_headerSymbols(headerSymbols)
	, m_lrData(lrData)
	, m_sentence(sentence)
	, m_logger(logger)
{
}

void SyntacticalAnalyzer::Run()
{
	m_currentLRData = m_lrData.front();
	m_stackLRData.push(m_currentLRData);
	std::set<std::string> possibleSymbols;

	while (!m_sentence.empty() || !m_stackLRData.empty())
	{
		Token currentToken = m_sentence.empty() ? m_stackSentence.top() : m_sentence.front();
		Symbol symbol = GetSymbolByChInLRData(currentToken.type);

		if (symbol.state == StateSymbol::Shift || (currentToken.value == "#" && symbol.number - 1 == 0 && m_sentence.size() == 1))
		{
			m_sentence.pop_front();
			m_stackSentence.push(currentToken);
			m_logger.Log(currentToken.type + "\tadd to INPUT CHARACTERS stack");
		}

		if (symbol.state == StateSymbol::Shift)
		{
			if (!IsNonterminal(currentToken.value))
			{
				possibleSymbols.clear();
			}

			m_currentLRData = m_lrData[symbol.number - 1];
			m_stackLRData.push(m_currentLRData);
			m_logger.Log(std::to_string(m_currentLRData.number) + "\tadd to STATE stack");
		}
		else if (symbol.state == StateSymbol::Convolution)
		{
			GuideSetsData guideSetsData = m_guideSets[symbol.number - 1];
			for (size_t i = 0; i < guideSetsData.terminals.size(); i++)
			{
				if (!IsNonterminal(guideSetsData.terminals[i]))
				{
					possibleSymbols.insert(guideSetsData.terminals[i]);
				}

				if (guideSetsData.terminals[i] != "e")
				{
					if (!m_stackSentence.empty())
					{
						m_logger.Log(m_stackSentence.top().type + "\tdeleted from INPUT CHARACTERS stack");
						m_stackSentence.pop();
					}
					if (!m_stackLRData.empty())
					{
						m_logger.Log(std::to_string(m_stackLRData.top().number) + "\tdeleted from STATE stack");
						m_stackLRData.pop();
					}
				}
			}

			Token token({ guideSetsData.nonterminal, guideSetsData.nonterminal, currentToken.lineNumber, currentToken.positionIndex });
			m_sentence.push_front(token);

			m_logger.Log(guideSetsData.nonterminal + "\tadd to start position in SENTENCE");

			if (!m_stackLRData.empty())
			{
				m_currentLRData = m_stackLRData.top();
			}
			else
			{
				m_currentLRData = m_lrData.front();
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
			m_logger.Log("State none for header character '" + currentToken.type + "' in row number " + std::to_string(m_currentLRData.number));
			m_logger.Print();

			throw std::invalid_argument("State none for header character: " + currentToken.type + "\ntoken value: " + currentToken.value + "\nline number: " + std::to_string(currentToken.lineNumber) + "\nposition index: " + std::to_string(currentToken.positionIndex) + "\nin row number: " + std::to_string(m_currentLRData.number));
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
	std::stack<Token> copySentenceStack(m_stackSentence);
	std::string str;

	while (!copySentenceStack.empty())
	{
		str.append(copySentenceStack.top().type + " ");
		copySentenceStack.pop();
	}
	m_logger.Log("INPUT CHARACTERS stack items: " + str);
}

void SyntacticalAnalyzer::LogSentenceInfo()
{
	std::string str;
	for (auto ch : m_sentence)
	{
		str.append(ch.type + " ");
	}
	m_logger.Log("SENTENCE items: " + str);
}

void SyntacticalAnalyzer::PrintStackLRData()
{
	if (m_stackLRData.empty() || (m_stackLRData.size() == 1 && m_stackLRData.top().ch == m_lrData[0].ch))
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
	if (m_stackSentence.empty() || m_stackSentence.size() == 1)
	{
		std::cout << "INPUT CHARACTERS stack is empty = OK" << std::endl;
	}
	else
	{
		std::cout << "The stack of INPUT CHARACTERS is not empty, it contains: ";
		while (!m_stackSentence.empty())
		{
			std::cout << m_stackSentence.top().type << ", ";
			m_stackSentence.pop();
		}
		std::cout << std::endl;
	}
}

void SyntacticalAnalyzer::PrintSentence()
{
	if (m_sentence.empty() || (m_sentence.size() == 1 && m_sentence.front().type == m_guideSets[0].nonterminal))
	{
		std::cout << "The INPUT SENTENCE is empty = OK" << std::endl;
	}
	else
	{
		std::cout << "The INPUT SENTENCE is not empty, it contains: ";
		for (auto ch : m_sentence)
		{
			std::cout << ch.type << ", ";
		}
		std::cout << std::endl;
	}
}