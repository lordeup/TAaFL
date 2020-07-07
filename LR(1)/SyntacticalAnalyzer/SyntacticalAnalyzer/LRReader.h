#pragma once
#include "LRData.h"
#include "Helper.h"
#include <sstream>
#include <fstream>
#include <list>

class LRReader
{
public:
	LRReader();
	void ReadSentence(std::ifstream& fileSentenceInput);
	void ReadTable(std::ifstream& fileTableInput);
	std::vector<LRData> GetLRData();
	std::list<std::string> GetSentence();
	std::vector<std::string> GetHeaderSymbols();
	
private: 
	std::vector<Symbol> GetSymbols(std::istringstream& iss);
	void InitHeaderSymbols(std::string line);

	std::vector<std::string> m_headerSymbols;
	std::vector<LRData> m_lrData;
	std::list<std::string> m_sentence;
};
