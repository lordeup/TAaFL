#pragma once
#include "Header.h"
#include "IAutomat.h"

class CAutomatMealy : public IAutomat
{
public:
	CAutomatMealy(std::ostream& output, const int inputSize, const int stateCount, const VectorEdge& inputEdge);
	~CAutomatMealy() = default;

	void GraphView() const override;
	void MinimizationAutomat() override;
	void PrintInfo() const override;

private:
	int m_inputSize;
	int m_stateCount;

	std::ostream& m_output;

	VectorEdge m_inputEdge;
	VectorEdge m_groupStateEdge;
	VectorEdge m_uniqueEdge;
	VectorEdge m_conformityGroupEdge;

	VectorInt m_outputState;
};