#pragma once
#include "Header.h"

bool IsCheckUniqueness(const VectorString& vec, const std::string str);
bool IsNonterminal(const std::string str);
std::string GetRandomNonterminal(const VectorString& vec);

void FillingData(std::istream& fileInput, VectorInputData& inputDatas, VectorString& nonterminals, VectorString& terminals);
void Forming(const VectorInputData& inputDatas, VectorOutputData& outputDatas, VectorString& nonterminals, VectorString& terminals);
void ActionsRightSide(const VectorInputData& inputDatas, VectorOutputData& outputDatas, VectorString& nonterminals, VectorString& terminals,
	const std::string nonterminal, const VectorInputData& temporaryVector, const bool isBlankCharacter);

void AddingGuideCharacters(VectorOutputData& outputDatas, const VectorString& nonterminals, const VectorString& terminals);
void BuildingFirstRelationship(VectorOutputData& outputDatas, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters);
void BuildingFirstPlusRelationship(std::vector<PairStringVectorPair>& transitions);
void SearchStartingTerminalsEmptyRules(VectorOutputData& outputDatas, const std::string parentNonterminal, const std::string nonterminal, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters);

void PrintInfoVector(std::ofstream& fileOutput, const VectorString& vec);
void PrintResult(std::ofstream& fileOutput, const VectorOutputData& outputDatas);

std::vector<PairStringBool>::iterator GetIteratorFindIfVector(std::vector<PairStringBool>& vec, const std::string str);
std::vector<PairStringVectorPair>::iterator GetIteratorFindIfVector(std::vector<PairStringVectorPair>& vec, const std::string str);