#include "Utils.h"

bool IsCheckUniqueness(const std::vector<std::string>& vec, const std::string str)
{
	return std::find(vec.begin(), vec.end(), str) == vec.end();
}

bool IsNonterminal(const std::string str)
{
	return !str.empty() && str.front() == '<' && str.back() == '>';
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
	const size_t length = GetRandomNumber(1, 7);

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