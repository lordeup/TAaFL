#pragma once
#include "Header.h"
#include "IAutomat.h"
#include <set>

class Minimization
{
public:
	Minimization(const size_t inputSize, const size_t stateCount, const VectorEdge& inputEdge, const Automat automat);
	Minimization(const size_t inputSize, const size_t stateCount, const VectorSize_t& outputCharacter, const DualVectorSize_t& state, const Automat automat);
	~Minimization() = default;

	VectorEdge MinimizationMealy();
	VectorSize_t MinimizationMoore();
	size_t GetOutputStateSize() const;
	VectorSize_t GetOutputCharacterMoore() const;

private:
	size_t m_stateCount, m_inputSize;

	Automat m_automat;

	VectorEdge m_inputEdge, m_outputState;

	VectorSize_t m_outputCharacter, m_outputCharacterMoore, m_outputStateMoore;

	DualVectorSize_t m_groupPrevious, m_groupVector, m_state;

	void FillOutputMealy(const VectorEdge& groupPreviousEdge);
	void FillOutputMoore(const VectorEdge& groupPreviousEdge);

	VectorEdge GettingGroupMoore();
	DualVectorSize_t GettingGroup(const VectorEdge& inputEdge);

	VectorEdge GettingGroupNext(const VectorEdge& outputState, DualVectorSize_t& groupPrevious, DualVectorSize_t& groupVector);
	VectorEdge GettingGroupPreviousEdge(DualVectorSize_t& groupPrevious, const DualVectorSize_t& groupVector, const VectorEdge& groupEdge);
	VectorEdge GettingGroupEdge(const DualVectorSize_t& groupOutputEdge);
};