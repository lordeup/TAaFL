#pragma once
#include "Utils.h"

std::vector<std::vector<std::string>> GetGenerateData(const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& characters);

std::vector<std::string> GetCharacters(const std::vector<std::string>& nonterminals, const std::vector<std::string>& terminals);

std::vector<TableData> GetRightSides(const std::vector<OutputDataGuideSets>& inputDatas);

std::vector<TableData> GetGuideCharacters(const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<TableData>& rightSides);
void RecordingNonterminalGuideSets(const std::string str, const std::string nonterminal,
	const std::vector<TableData>& rightSides, std::vector<TableData>& tableDataGuideCharacters, std::vector<std::string>& copyGuideCharacters);

void AddShift(const TableData& tableData, size_t& count, const std::vector<std::string>& characters, std::vector<std::string>& vec, std::vector<TableData>& states, std::stack<TableData>& stack);

void FindRollUp(const std::string nonterminal, const size_t index, const std::vector<TableData>& rightSides,
	const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& characters, std::vector<std::string>& vec);
void AddRollUp(const std::string str, const size_t index, const std::vector<std::string>& characters, std::vector<std::string>& vec);

void PrintResult(std::ofstream& fileOutput, const std::vector<std::vector<std::string>>& outputDatas, const std::vector<std::string>& characters);
