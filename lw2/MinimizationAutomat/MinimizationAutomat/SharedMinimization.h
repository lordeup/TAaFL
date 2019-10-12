#pragma once
#include "Header.h"
#include "IAutomat.h"
#include <boost/functional/hash.hpp>
#include <unordered_set>

class SharedMinimization
{
public:
	SharedMinimization(const int inputSize, const int stateCount, const VectorEdge& inputEdge, const Automat automat);
	SharedMinimization(const int inputSize, const int stateCount, const VectorInt& outputCharacter, const std::vector<VectorInt>& state, const Automat automat);
	~SharedMinimization() = default;

	VectorEdge Minimization();
	int GetOutputStateSize();

private:
	VectorEdge GetConformityPreviousGroupEdge(DualVectorInt& conformityGroupVectorPrevious, DualVectorInt& conformityGroupVector, VectorEdge& conformityGroupEdge);
	void FillOutput(const DualVectorInt& conformityGroupVectorPrevious, const VectorEdge& conformityPreviousGroupEdge);

	VectorEdge GettingGroupOutputEdgeMealy(const VectorEdge& inputEdge);
	VectorEdge GettingGroupOutputEdgeMoore(const VectorInt& outputCharacter, const VectorInt& uniqueItem);

	VectorEdge GettingConformityGroupEdge(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, DualVectorInt& conformityGroupVector);

	VectorEdge GettingUniqueEdgeNext(const VectorEdge& groupOutputState, const DualVectorInt& conformityGroupVector);

	VectorEdge GettingConformityGroupEdgeNext(const VectorEdge& groupOutputEdge, const VectorEdge& uniqueEdge, const VectorEdge& conformityGroupEdgeStart, DualVectorInt& conformityGroupVector);

	int m_stateCount;
	int m_inputSize;
	DualVectorInt m_conformityGroupVectorPrevious;
	DualVectorInt m_conformityGroupVector;

	VectorEdge m_inputEdge;
	VectorEdge m_outputState;
	Automat m_automat;

	std::vector<VectorInt> m_state;
	VectorInt m_outputCharacter;
};