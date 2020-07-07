#pragma once
#include "LRData.h"
#include <fstream>
#include <stack>
#include <list>
#include <algorithm>

class SyntacticalAnalyzer
{
public:
	SyntacticalAnalyzer(std::vector<std::string> headerSymbols, std::vector<LRData> lrData, std::list<std::string> sentence);
	void Run();

private:
	LRData GetLRDataByCh(std::string currentChar);
	Symbol GetSymbolByChInLRData(std::string headerSymbol);
	int GetIndexInHeaderSymbols(std::string headerSymbol);

	std::vector<std::string> m_headerSymbols;
	std::vector<LRData> m_lrData;
	std::list<std::string> m_sentence;
	std::stack<std::string> m_stackCh;
	std::stack<std::string> m_stackSentence;
	LRData m_currentLRData;
};
