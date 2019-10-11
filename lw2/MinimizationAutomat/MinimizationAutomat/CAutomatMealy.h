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
	VectorEdge GettingGroupOutputEdge(const VectorEdge& inputEdge);
	VectorEdge GettingUniqueEdge(const VectorEdge& groupOutputState);
	VectorEdge GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	VectorEdge GettingUniqueEdgeNext(const VectorEdge& groupOutputState, const DualVectorInt& conformityGroupVector);
	VectorEdge GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, const VectorEdge& conformityGroupEdgeStart, DualVectorInt& conformityGroupVector);

	int m_inputSize;
	int m_stateCount;

	std::ostream& m_output;

	VectorEdge m_inputEdge;
	VectorInt m_outputState;
};