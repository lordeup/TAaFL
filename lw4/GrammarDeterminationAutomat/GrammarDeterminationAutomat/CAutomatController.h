#pragma once
#include "Automat.h"
#include <sstream>

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void ProcessingCommand();

private:
	std::istream& m_input;
	std::ostream& m_output;

	MapEdge m_stateMap;
	Grammar m_grammar;

	void FillingData(const size_t inputSize, const size_t stateCount, const size_t rulesCount);
	void DefinitionGrammar();
	std::string GetConvertToString(const char ch) const;
};