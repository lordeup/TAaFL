#pragma once
#include "CAutomatMealy.h"
#include "CAutomatMoore.h"

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void ProcessingCommand();

private:
	std::istream& m_input;
	std::ostream& m_output;

	Automat m_automat;

	size_t SearchNumberInStringMoore(const std::string str);
	Edge SearchNumberInStringMealy(const std::string str);

	void SetAutomat(const std::string automat);

	DualVectorSize_t FillingDataMoore(const size_t inputSize, const size_t stateCount);

	VectorEdge FillingDataMealy(const size_t inputSize, const size_t stateCount);

	VectorSize_t FillOutputState(const size_t stateCount);
};