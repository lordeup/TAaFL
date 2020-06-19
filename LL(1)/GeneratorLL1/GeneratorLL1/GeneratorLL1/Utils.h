#pragma once
#include <fstream>
#include <string>
#include <vector>

using PairStringBool = std::pair<std::string, bool>;
using PairStringVectorPair = std::pair<std::string, std::vector<PairStringBool>>;

bool IsCheckUniqueness(const std::vector<std::string>& vec, const std::string str);
bool IsNonterminal(const std::string str);

std::string GetRandomString();
size_t GetRandomNumber(const size_t min, const size_t max);
std::string SubstrNonterminal(const std::string str);

void PrintInfoVector(std::ostream& fileOutput, const std::vector<std::string>& vec, const std::string str);

std::vector<PairStringBool>::iterator GetIteratorFindIfVector(std::vector<PairStringBool>& vec, const std::string str);
std::vector<PairStringVectorPair>::iterator GetIteratorFindIfVector(std::vector<PairStringVectorPair>& vec, const std::string str);