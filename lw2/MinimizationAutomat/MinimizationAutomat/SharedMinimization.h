#pragma once
#include "Header.h"
#include "IAutomat.h"
#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <set>

class SharedMinimization
{
public:
	SharedMinimization(const int inputSize, const int stateCount, const VectorEdge& inputEdge, const Automat automat);
	SharedMinimization(const int inputSize, const int stateCount, const VectorInt& outputCharacter, const DualVectorInt& state, const Automat automat);
	~SharedMinimization() = default;

	VectorEdge MinimizationMealy();
	VectorInt MinimizationMoore();
	int GetOutputStateSize() const;
	VectorInt GetOutputCharacterMoore() const;

private:
	VectorEdge GetConformityPreviousGroupEdge(DualVectorInt& conformityGroupVectorPrevious, const DualVectorInt& conformityGroupVector, const VectorEdge& conformityGroupEdge);
	void FillOutput(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);
	void FillOutputMoore(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);

	VectorEdge GettingUniqueMealy(const VectorEdge& groupOutputEdge, const int size);
	VectorInt GettingUniqueMoore(const VectorInt& groupOutputEdge);

	VectorEdge GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge);
	VectorEdge GettingGroupOutputEdgeMoore(const VectorInt& outputCharacter, const VectorInt& uniqueItem, DualVectorInt& conformityGroupVector);

	VectorEdge GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	VectorEdge GettingUniqueEdgeNext(const VectorEdge& groupOutputState, const DualVectorInt& conformityGroupVector);

	VectorEdge GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	int m_stateCount;
	int m_inputSize;
	DualVectorInt m_conformityGroupVectorPrevious;
	DualVectorInt m_conformityGroupVector;

	VectorEdge m_inputEdge;
	VectorEdge m_outputState;
	Automat m_automat;

	DualVectorInt m_state;
	VectorInt m_outputCharacter;

	VectorInt m_outputCharacterMoore;
	VectorInt m_outputStateMoore;
};