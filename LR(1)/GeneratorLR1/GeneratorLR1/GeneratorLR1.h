#pragma once
#include "Header.h"

bool IsNonterminal(const std::string str);
bool IsCheckUniqueness(const VectorString& vec, const std::string str);
bool IsCheckTableDataUniqueness(const std::vector<TableData>& vec, const TableData& tableData);
bool operator==(const TableData& a, const TableData& b);
size_t GetDistanceVector(const VectorString& vec, const std::string str);

void FillingData(std::istream& fileInput, std::vector<InputData>& inputDatas, VectorString& characters);
void Generate(const std::vector<InputData>& inputDatas, const VectorString& characters, std::vector<VectorString>& outputDatas);

std::vector<TableData> GetRightSides(const std::vector<InputData>& inputDatas);

std::vector<TableData> GetGuideCharacters(const std::vector<InputData>& inputDatas, const std::vector<TableData>& rightSides);
void RecordingNonterminalGuideSets(const std::string str, const std::string nonterminal, const std::vector<TableData>& rightSides, std::vector<TableData>& tableDataGuideCharacters, VectorString& copyGuideCharacters);

void AddShift(const TableData& tableData, size_t& count, const VectorString& characters, VectorString& vec, std::vector<TableData>& states, std::stack<TableData>& stack);

void FindRollUp(const std::string nonterminal, const size_t index, const std::vector<TableData>& rightSides, const std::vector<InputData>& inputDatas, const VectorString& characters, VectorString& vec);
void AddRollUp(const std::string str, const size_t index, const VectorString& characters, VectorString& vec);

void PrintInfoVector(std::ofstream& fileOutput, const std::vector<std::string>& vec);
void PrintResult(std::ofstream& fileOutput, const std::vector<VectorString>& outputDatas, const VectorString& characters);
