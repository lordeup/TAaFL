#pragma once
#include "Header.h"

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void DataReading();

private:
	enum class Automat
	{
		UNKNOWN,
		MOORE,
		MEALY
	};

	void SetAutomat(const std::string automat);
	void PrintInfo(const EdgeVector& edge) const;
	void PrintChart() const;

	void FillingDataMoore();
	void TransferAutomatMealy();

	void FillingDataMealy();
	void TransferAutomatMoore();

	std::istream& m_input;
	std::ostream& m_output;

	int m_inputSize;
	int m_outputSize;
	int m_stateCount;
	Automat m_automat;

	EdgeVector m_mealyEdge;

	std::vector<IntVector> m_state;
	EdgeVector m_edge;
	IntVector m_outputCharacter;
};