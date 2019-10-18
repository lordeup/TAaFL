#pragma once
#include "Minimization.h"
#include "Visualization.h"

class CAutomatMoore : public IAutomat
{
public:
	CAutomatMoore(std::ostream& output, const size_t inputSize, const size_t stateCount, const VectorSize_t& outputCharacter, const DualVectorSize_t& state);
	~CAutomatMoore() = default;

	void GraphView() const override;
	void MinimizationAutomat() override;
	void PrintInfo() const override;

private:
	size_t m_stateCount, m_inputSize, m_outputStateSize;

	VectorSize_t m_outputCharacter, m_outputCharacterMoore, m_outputStateMoore;

	DualVectorSize_t m_state;

	std::ostream& m_output;
};