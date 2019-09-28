#pragma once
#include "CAutomatMealy.h"
#include "CAutomatMoore.h"

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void ProcessingCommand();

private:
	enum class Automat
	{
		UNKNOWN,
		MOORE,
		MEALY
	};

	void SetAutomat(const std::string automat);

	std::vector<VectorInt> FillingDataMoore(const int inputSize, const int stateCount);

	VectorEdge FillingDataMealy(const int inputSize, const int stateCount);

	VectorInt FillOutputState(const int stateCount);

	std::istream& m_input;
	std::ostream& m_output;

	Automat m_automat;
};