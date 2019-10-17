#pragma once
#include "Header.h"
#include "IAutomat.h"
#include <set>

class Minimization
{
public:
	Minimization(const size_t inputSize, const size_t stateCount, const VectorEdge& inputEdge);
	Minimization(const size_t inputSize, const size_t stateCount, const VectorSize_t& outputCharacter, const DualVectorSize_t& state);
	~Minimization() = default;

	VectorEdge MinimizationMealy();
	VectorSize_t MinimizationMoore();
	size_t GetOutputStateSize() const;
	VectorSize_t GetOutputCharacterMoore() const;

private:
	VectorEdge GetConformityPreviousGroupEdge(DualVectorSize_t& conformityGroupVectorPrevious, const DualVectorSize_t& conformityGroupVector, const VectorEdge& conformityGroupEdge);
	void FillOutput(const DualVectorSize_t& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);
	void FillOutputMoore(const DualVectorSize_t& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);

	DualVectorSize_t GetTest(const VectorEdge& inputEdge);
	DualVectorSize_t GettingUniqueMealy(const DualVectorSize_t& groupOutputEdge);
	VectorEdge GettingConformityGroupEdge(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& uniqueEdge);
	VectorEdge GetPrevTest(DualVectorSize_t& conformityGroupVectorPrevious, const DualVectorSize_t& conformityGroupVector, const VectorEdge& conformityGroupEdge);
	DualVectorSize_t GetNextUnicTest(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& conformityGroupVector);

	VectorSize_t GettingUniqueMoore(const VectorSize_t& groupOutputEdge);

	VectorEdge GeConfGrouptTest(const DualVectorSize_t& groupOutputEdge, const DualVectorSize_t& uniqueEdge, DualVectorSize_t& conformityGroupVector);

	VectorEdge GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge);
	VectorEdge GettingGroupOutputEdgeMoore(const VectorSize_t& outputCharacter, const VectorSize_t& uniqueItem, DualVectorSize_t& conformityGroupVector);

	VectorEdge GettingUniqueEdgeNext(const VectorEdge& groupOutputState, const DualVectorSize_t& conformityGroupVector);

	VectorEdge GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorSize_t& conformityGroupVector);

	size_t m_stateCount;
	size_t m_inputSize;
	DualVectorSize_t m_conformityGroupVectorPrevious;
	DualVectorSize_t m_conformityGroupVector;

	VectorEdge m_inputEdge;

	VectorEdge m_outputState;

	DualVectorSize_t m_state;
	VectorSize_t m_outputCharacter;

	VectorSize_t m_outputCharacterMoore;
	VectorSize_t m_outputStateMoore;
};