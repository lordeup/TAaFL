#pragma once
#include "Automat.h"

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void ProcessingCommand();

private:
	std::istream& m_input;
	std::ostream& m_output;

	DualVectorSize_t FillingData(const size_t inputSize, const size_t stateCount);
	VectorSize_t SearchNumberInString(std::string str);
};