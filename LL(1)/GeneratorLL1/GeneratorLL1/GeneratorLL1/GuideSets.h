#pragma once
#include "Lexer.h"
#include "Utils.h"

std::vector<OutputDataGuideSets> GetFormingGuideSets(std::istream& fileInput);

std::string GetTokenType(std::string str, std::vector<PairStringString>& pairsTerminalTokenType, Lexer& lexer);

void FillingData(std::istream& fileInput, std::vector<InputData>& inputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals, Lexer& lexer);
void Forming(const std::vector<InputData>& inputDatas, std::vector<OutputDataGuideSets>& outputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals);
void ActionsRightSide(const std::vector<InputData>& inputDatas, std::vector<OutputDataGuideSets>& outputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals,
	const std::string nonterminal, const std::vector<InputData>& temporaryVector, const bool isBlankCharacter);

void AddingGuideCharacters(std::vector<OutputDataGuideSets>& outputDatas, const std::vector<std::string>& nonterminals, const std::vector<std::string>& terminals);
void BuildingFirstRelationship(std::vector<OutputDataGuideSets>& outputDatas, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters);
void BuildingFirstPlusRelationship(std::vector<PairStringVectorPair>& transitions);
void SearchStartingTerminalsEmptyRules(std::vector<OutputDataGuideSets>& outputDatas, const std::string parentNonterminal, const std::string nonterminal, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters);

void PrintResultGuideSets(std::ofstream& fileOutput, const std::vector<OutputDataGuideSets>& outputDatas);