#pragma once
#include "Header.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

struct InputData
{
	std::string nonterminal;
	std::vector<std::string> terminals;
};

struct OutputData
{
	std::string nonterminal;
	std::vector<std::string> terminals;
	std::vector<std::string> guideCharacters;
};

using PairStringBool = std::pair<std::string, bool>;
using PairStringVectorPair = std::pair<std::string, std::vector<PairStringBool>>;

class GuideSets
{
public:
	GuideSets(std::istream& input, std::ostream& output);

	void Execute();

private:
	std::istream& m_input;
	std::ostream& m_output;

	std::vector<std::string> m_nonterminals;
	std::vector<std::string> m_terminals;

	std::vector<InputData> m_inputData;
	std::vector<OutputData> m_outputData;

	void FillingData();
	void Forming();
	void ActionsRightSide(const std::string nonterminal, const std::vector<InputData>& temporaryVector);

	void BuildingFirstRelationship(std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& ñharacters);
	void SearchStartingTerminalsEmptyRules(const std::string parentNonterminal, const std::string nonterminal, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& ñharacters);
	void BuildingFirstPlusRelationship(std::vector<PairStringVectorPair>& transitions);
	void AddingGuideCharacters();

	void PrintResult();

	std::string GetRandomNonterminal() const;
	void PrintInfoVector(const std::vector<std::string>& vec) const;
	bool IsNonterminal(const std::string str) const;

	std::vector<std::string> GetUniques(const std::vector<std::string>& collection) const;

	std::vector<PairStringBool>::iterator GetIteratorFindIfVector(std::vector<PairStringBool>& vec, const std::string str) const;
	std::vector<PairStringVectorPair>::iterator GetIteratorFindIfVector(std::vector<PairStringVectorPair>& vec, const std::string str) const;
};