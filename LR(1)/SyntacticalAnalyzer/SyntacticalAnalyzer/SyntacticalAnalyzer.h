#pragma once
#include "LRData.h"
#include "Helper.h"
#include <fstream>
#include <list>
#include <algorithm>
#include <iostream>

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
	std::stack<LRData> m_stackLRData;
	std::stack<std::string> m_stackSentence;
	LRData m_currentLRData;
};
