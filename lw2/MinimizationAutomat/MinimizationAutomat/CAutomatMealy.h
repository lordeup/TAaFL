#pragma once
#include "Minimization.h"
#include "Visualization.h"

class CAutomatMealy : public IAutomat
{
public:
	CAutomatMealy(std::ostream& output, const size_t inputSize, const size_t stateCount, const VectorEdge& inputEdge);
	~CAutomatMealy() = default;

	void GraphView() const override;
	void MinimizationAutomat() override;
	void PrintInfo() const override;

private:
	size_t m_inputSize;
	size_t m_stateCount;
	size_t m_outputStateSize;

	std::ostream& m_output;

	VectorEdge m_inputEdge;
	VectorEdge m_outputState;
};