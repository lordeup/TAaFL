#pragma once
#include "Header.h"

enum class Grammar
{
	LEFT,
	RIGHT
};

class Automat
{
public:
	Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const size_t rulesCount, const MapEdge& stateMap, const Grammar grammar);
	~Automat() = default;

	void GraphView();
	void Determination();
	void PrintInfo() const;

private:
	size_t m_inputSize, m_stateCount, m_rulesCount;

	Grammar m_grammar;
	MapEdge m_stateMap;
	VectorString m_term, m_noTerm;

	DualVectorString m_determinationStateString;
	TripleVectorPairString m_inputStateChar;
	TripleVectorString m_outputStateString;

	std::ostream& m_output;

	void UniqueVector(DualVectorString& columnVector);
	void DeterminationRightGrammar();
	void DeterminationLeftGrammar();
	void AddDetermination();
	void UniqueNoTerm(VectorString& columnVector);
	VectorString ConvertVertexString();
	VectorPairString ConvertStateString();
};