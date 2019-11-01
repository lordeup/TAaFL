#pragma once
#include "Header.h"
#include "IAutomat.h"

class Automat : public IAutomat
{
public:
	Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const DualVectorSize_t& inputState);
	~Automat() = default;

	void GraphView() override;
	void Determination() override;
	void PrintInfo() const override;

private:
	size_t m_inputSize, m_stateCount;

	DualVectorSize_t m_inputState, m_determinationState;
	TripleVectorSize_t m_outputState;

	std::ostream& m_output;

	void FillingColumns();
	void CheckDetermination();
	void UniqueVector(DualVectorSize_t& columnVector);
	void FindUniqueCell(const VectorSize_t& cell);
	VectorString ConvertStateString();
};