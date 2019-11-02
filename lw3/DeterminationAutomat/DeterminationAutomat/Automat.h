#pragma once
#include "Header.h"

class Automat
{
public:
	Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const DualVectorSize_t& inputState);
	~Automat() = default;

	void GraphView();
	void Determination();
	void PrintInfo() const;

private:
	size_t m_inputSize, m_stateCount;

	DualVectorSize_t m_inputState, m_determinationState;
	TripleVectorSize_t m_outputState, m_inputStateColumn;

	std::ostream& m_output;

	void UniqueVector(DualVectorSize_t& columnVector);
	VectorString ConvertStateString();
	VectorString ConvertVertexString();
};