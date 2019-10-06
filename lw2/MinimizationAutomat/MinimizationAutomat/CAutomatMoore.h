#pragma once
#include "Header.h"
#include "IAutomat.h"

class CAutomatMoore : public IAutomat
{
public:
	CAutomatMoore(std::ostream& output, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state);
	~CAutomatMoore() = default;

	void GraphView() const override;
	void MinimizationAutomat() override;
	void PrintInfo() const override;

private:
	int m_stateCount;

	std::ostream& m_output;

	std::vector<VectorInt> m_state;
	VectorEdge m_edge;
	VectorInt m_outputCharacter;
};