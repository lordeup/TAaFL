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


	std::istream& m_input;
	std::ostream& m_output;

	int m_sizeInputCharacter;
	int m_sizeOutputCharacter;
	int m_countVertice;
	Automat m_automat;
	std::vector<Edge> m_edges;
};