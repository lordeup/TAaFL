#pragma once
#include "Helper.h"
#include "LRData.h"
#include "Lexer.h"
#include <fstream>
#include <list>
#include <sstream>

class LRReader
{
public:
	LRReader();
	void ReadGuideSets(std::ifstream& fileGuideSetsInput);
	void ReadSentence(std::ifstream& fileSentenceInput);
	void ReadTable(std::ifstream& fileTableInput);
	std::vector<GuideSetsData> GetGuideSets();
	std::vector<LRData> GetLRData();
	std::list<Token> GetSentence();
	std::vector<std::string> GetHeaderSymbols();

private:
	std::vector<Symbol> GetSymbols(std::istringstream& iss);
	void InitHeaderSymbols(std::string line);

	std::vector<GuideSetsData> m_guideSets;
	std::vector<std::string> m_headerSymbols;
	std::vector<LRData> m_lrData;
	std::list<Token> m_sentence;
};
