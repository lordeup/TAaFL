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

	bool IsFillingDataMoore();
	void TransferAutomatMealy();
	void PrintInfoTransferMoore();

	bool IsFillingDataMealy();
	void TransferAutomatMoore();
	void PrintInfoTransferMealy();

	std::istream& m_input;
	std::ostream& m_output;

	int m_sizeInputCharacter;
	int m_sizeOutputCharacter;
	int m_countVertice;
	Automat m_automat;

	std::vector<Edge> m_mealy_edge;
	std::vector<Edge> m_mealy_edge_output;

	std::vector<std::vector<int>> m_state;
	std::vector<Edge> m_edge;
	std::vector<int> m_outputCharacter;
};