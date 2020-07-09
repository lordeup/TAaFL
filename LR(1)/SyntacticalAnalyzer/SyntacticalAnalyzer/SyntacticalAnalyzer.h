#pragma once
#include "LRData.h"
#include "Lexer.h"
#include "Logger.h"
#include "Helper.h"
#include <set>
#include <fstream>
#include <list>
#include <algorithm>
#include <iostream>

class SyntacticalAnalyzer
{
public:
	SyntacticalAnalyzer(std::vector<GuideSetsData> guideSets, std::vector<std::string> headerSymbols, std::vector<LRData> lrData, std::list<Token> sentence);
	void Run();

private:
	LRData GetLRDataByCh(std::string currentChar);
	Symbol GetSymbolByChInLRData(std::string headerSymbol);
	size_t GetIndexInHeaderSymbols(std::string headerSymbol);

	void LogStackLRDataInfo();
	void LogStackSentenceInfo();
	void LogSentenceInfo();

	void PrintStackLRData();
	void PrintStackSentence();
	void PrintSentence();

	std::vector<GuideSetsData> m_guideSets;
	std::vector<std::string> m_headerSymbols;
	std::vector<LRData> m_lrData;
	std::list<Token> m_sentence;
	std::stack<LRData> m_stackLRData;
	std::stack<Token> m_stackSentence;
	LRData m_currentLRData;
	Logger m_logger;
};
