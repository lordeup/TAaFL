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
	void ReadGuideSets(std::ifstream& fileGuideSetsInput);
	void ReadSentence(std::ifstream& fileSentenceInput);
	void ReadTable(std::ifstream& fileTableInput);
	std::vector<GuideSetsData> GetGuideSets();
	std::vector<LRData> GetLRData();
	std::list<std::string> GetSentence();
	std::vector<std::string> GetHeaderSymbols();
	
private: 
	std::vector<Symbol> GetSymbols(std::istringstream& iss);
	void InitHeaderSymbols(std::string line);

	std::vector<GuideSetsData> m_guideSets;
	std::vector<std::string> m_headerSymbols;
	std::vector<LRData> m_lrData;
	std::list<std::string> m_sentence;
};
