#pragma once
#include "Header.h"
#include "IAutomat.h"
#include <set>

class Minimization
{
public:
	Minimization(const int inputSize, const int stateCount, const VectorEdge& inputEdge, const Automat automat);
	Minimization(const int inputSize, const int stateCount, const VectorInt& outputCharacter, const DualVectorInt& state, const Automat automat);
	~Minimization() = default;

	VectorEdge MinimizationMealy();
	VectorInt MinimizationMoore();
	int GetOutputStateSize() const;
	VectorInt GetOutputCharacterMoore() const;

	int GetOutputStateSizeTest() const;

private:
	VectorEdge GetConformityPreviousGroupEdge(DualVectorInt& conformityGroupVectorPrevious, const DualVectorInt& conformityGroupVector, const VectorEdge& conformityGroupEdge);
	void FillOutput(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);
	void FillOutputMoore(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);

	DualVectorInt GetTest(const VectorEdge& inputEdge);
	DualVectorInt GetUniqueTest(const DualVectorInt& groupOutputEdge);
	VectorEdge GetConfTest(const DualVectorInt& groupOutputEdge, const DualVectorInt& uniqueEdge);
	VectorEdge GetPrevTest(DualVectorInt& conformityGroupVectorPrevious, const DualVectorInt& conformityGroupVector, const VectorEdge& conformityGroupEdge);
	DualVectorInt GetNextUnicTest(const DualVectorInt& groupOutputEdge, const DualVectorInt& conformityGroupVector);

	VectorEdge GettingUniqueMealy(const VectorEdge& groupOutputEdge, const int size);
	VectorInt GettingUniqueMoore(const VectorInt& groupOutputEdge);

	VectorEdge GeConfGrouptTest(const DualVectorInt& groupOutputEdge, const DualVectorInt& uniqueEdge, DualVectorInt& conformityGroupVector);

	VectorEdge GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge);
	VectorEdge GettingGroupOutputEdgeMoore(const VectorInt& outputCharacter, const VectorInt& uniqueItem, DualVectorInt& conformityGroupVector);

	VectorEdge GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	VectorEdge GettingUniqueEdgeNext(const VectorEdge& groupOutputState, const DualVectorInt& conformityGroupVector);

	VectorEdge GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	int m_stateCount;
	int m_inputSize;
	DualVectorInt m_conformityGroupVectorPrevious;
	DualVectorInt m_conformityGroupVector;

	DualVectorInt m_confTest;
	DualVectorInt m_prevTest;

	VectorEdge m_inputEdge;
	std::vector<VectorEdge> m_inputMatrix;

	VectorEdge m_outputState;
	Automat m_automat;

	DualVectorInt m_state;
	VectorInt m_outputCharacter;

	VectorInt m_outputCharacterMoore;
	VectorInt m_outputStateMoore;
};