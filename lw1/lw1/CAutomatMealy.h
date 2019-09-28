#pragma once
#include "IAutomat.h"

class CAutomatMealy : public IAutomat
{
public:
	CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, VectorEdge& edge);
	~CAutomatMealy() = default;

	void GraphView() override;
	void TransferAutomat() override;
	void PrintInfo() override;

private:
	int m_inputSize;
	int m_stateCount;

	std::ostream& m_output;

	VectorEdge m_mealyEdge;
	VectorEdge m_copyEdge;

	std::vector<VectorInt> m_state;
	VectorEdge m_edge;
	VectorInt m_outputCharacter;
};
