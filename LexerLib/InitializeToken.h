#pragma once
#include "Header.h"
#include "TokenType.h"

struct TokenDataString
{
	std::string type;
	std::string value;
};

struct TokenDataChar
{
	std::string type;
	char value;
};

class InitializeToken
{
public:
	VectorString GetComment();
	std::vector<TokenDataString> GetOperator();

	std::vector<TokenDataString> GetLogicalOperator();
	std::vector<TokenDataString> GetBitwiseOperator();
	std::vector<TokenDataChar> GetArithmeticOperator();
	std::vector<TokenDataString> GetComparisonsOperator();

	std::vector<TokenDataChar> GetBracket();
	std::vector<TokenDataChar> GetSeparator();
	VectorChar GetBinary();
	VectorChar GetOctal();
	VectorChar GetDecimalFractional();
	VectorChar GetHexadecimal();
};