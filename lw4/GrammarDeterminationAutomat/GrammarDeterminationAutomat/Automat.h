#pragma once
#include "Header.h"

enum class Grammar
{
	UNKNOWN,
	LEFT,
	RIGHT
};

class Automat
{
public:
	Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const MapEdge& stateMap, const Grammar grammar);
	~Automat() = default;

	void GraphView();
	void Determination();
	void PrintInfo() const;

private:
	size_t m_inputSize, m_stateCount;

	Grammar m_grammar;
	MapEdge m_stateMap;

	DualVectorString m_determinationState;
	DualVectorPairString m_inputState;
	TripleVectorString m_outputState;

	MapKeyString m_noTermimalKeyString, m_termimalKeyString;
	MapKeySize_t m_noTermimalKeySize_t, m_termimalKeySize_t;

	std::ostream& m_output;

	bool isUniqueVector(DualVectorString& columnVector, DualVectorString& vectorPush);

	void AddDeterminationRight();
	void AddDeterminationLeft();

	void DeterminationRightGrammar();
	void DeterminationLeftGrammar();

	VectorString ConvertVertexString();
	VectorPairString ConvertStateString();
};