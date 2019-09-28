#pragma once
#include "Header.h"
#include "IAutomat.h"

class CAutomatMealy : public IAutomat
{
public:
	CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, const VectorEdge& edge);
	~CAutomatMealy() = default;

	void GraphView() const override;
	void TransferAutomat() override;
	void PrintInfo() const override;

private:
	int m_inputSize;
	int m_stateCount;

	std::ostream& m_output;

	VectorEdge m_edge;
	VectorEdge m_mealyEdge;
	VectorEdge m_copyEdge;

	std::vector<VectorInt> m_state;
	VectorInt m_outputCharacter;
};
