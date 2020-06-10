#pragma once
#include "Header.h"

bool IsNonterminal(const std::string str);
bool IsCheckUniqueness(const VectorString& vec, const std::string str);
void FillingData(std::istream& fileInput, std::vector<InputData>& inputDatas, VectorString& nonterminals, VectorString& terminals);
void Generate(const std::vector<InputData>& inputDatas, std::vector<VectorString>& outputDatas, const VectorString& nonterminals, const VectorString& terminals, VectorString& characters);

void AddShift(VectorString& temporaryVector, VectorString& characters, std::queue<std::string>& queue, const std::string str, size_t& count);

void PrintInfoVector(std::ofstream& fileOutput, const std::vector<std::string>& vec);
void PrintResult(std::ofstream& fileOutput, const std::vector<VectorString>& outputDatas, const VectorString& characters);

size_t GetDistanceVector(VectorString& vec, const std::string str);