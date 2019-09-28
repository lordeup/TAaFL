#pragma once
#include "IAutomat.h"

class CAutomatMoore : public IAutomat
{
public:
	CAutomatMoore(std::ostream& output, const int stateCount, VectorInt& outputState, std::vector<VectorInt>& state);
	~CAutomatMoore() = default;

	void GraphView() override;
	void TransferAutomat() override;
	void PrintInfo() override;

private:
	int m_stateCount;

	std::ostream& m_output;

	std::vector<VectorInt> m_state;
	VectorEdge m_edge;
	VectorInt m_outputState;
};
