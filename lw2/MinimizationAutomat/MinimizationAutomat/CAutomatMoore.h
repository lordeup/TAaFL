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
	size_t m_stateCount;
	size_t m_inputSize;
	size_t m_outputStateSize;

	std::ostream& m_output;

	DualVectorSize_t m_state;
	VectorSize_t m_outputCharacter;

	VectorSize_t m_outputCharacterMoore;
	VectorSize_t m_outputStateMoore;
};