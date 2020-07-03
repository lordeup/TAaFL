#include "Utils.h"

bool IsCheckUniqueness(const std::vector<std::string>& vec, const std::string str)
{
	return std::find(vec.begin(), vec.end(), str) == vec.end();
}

bool IsNonterminal(const std::string str)
{
	return !str.empty() && str.front() == '<' && str.back() == '>';
}

bool IsEmptyRule(const std::string str)
{
	return str == NONTERMINAL_END_SEQUENCE;
}

size_t GetRandomNumber(const size_t min, const size_t max)
{
	return (std::rand() % max) + min;
}

std::string SubstrNonterminal(const std::string str)
{
	return str.substr(1, str.length() - 2);
}

std::string GetRandomString()
{
	const std::string characters = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const size_t length = GetRandomNumber(1, 5);

	std::string str;

	for (size_t i = 0; i < length; ++i)
	{
		size_t random = std::rand() % characters.size();
		str += characters[random];
	}

	return str;
}

void PrintInfoVector(std::ostream& fileOutput, const std::vector<std::string>& vec, const std::string str)
{
	if (!vec.empty())
	{
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(fileOutput, str.c_str()));
		fileOutput << vec.back();
	}
}

std::vector<PairStringBool>::iterator GetIteratorFindIfVector(std::vector<PairStringBool>& vec, const std::string str)
{
	return std::find_if(vec.begin(), vec.end(), [&](const PairStringBool& data) { return data.first == str; });
}

std::vector<PairStringVectorPair>::iterator GetIteratorFindIfVector(std::vector<PairStringVectorPair>& vec, const std::string str)
{
	return std::find_if(vec.begin(), vec.end(), [&](const PairStringVectorPair& data) { return data.first == str; });
}

size_t GetDistanceVector(const std::vector<std::string>& vec, const std::string str)
{
	return std::distance(vec.begin(), std::find_if(vec.begin(), vec.end(), [&](const std::string data) { return data == str; }));
}

bool operator==(const TableData& a, const TableData& b)
{
	return a.character == b.character && a.position == b.position && a.row == b.row;
}

bool IsCheckTableDataUniqueness(const std::vector<TableData>& vec, const TableData& tableData)
{
	return std::find_if(vec.begin(), vec.end(), [&](const TableData& data) { return data == tableData; }) == vec.end();
}