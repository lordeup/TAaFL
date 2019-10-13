#pragma once
#include "SharedMinimization.h"
#include "Visualization.h"

class CAutomatMoore : public IAutomat
{
public:
	CAutomatMoore(std::ostream& output, const int inputSize, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state);
	~CAutomatMoore() = default;

	void GraphView() const override;
	void MinimizationAutomat() override;
	void PrintInfo() const override;

private:
	int m_stateCount;
	int m_inputSize;
	int m_outputStateSize;

	std::ostream& m_output;

	std::vector<VectorInt> m_state;
	VectorInt m_outputCharacter;

	VectorInt m_outputCharacterMoore;
	VectorInt m_outputStateMoore;
};