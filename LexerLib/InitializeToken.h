#pragma once
#include "Header.h"

class InitializeToken
{
public:
	InitializeToken();

	VectorString GetComment();
	VectorString GetOperator();

	VectorString GetLogicalOperator();
	VectorString GetBitwiseOperator();
	VectorChar GetArithmeticOperator();
	VectorString GetComparisonsOperator();

	VectorChar GetBracket();
	VectorChar GetSeparator();
	VectorChar GetBinary();
	VectorChar GetOctal();
	VectorChar GetDecimalFractional();
	VectorChar GetHexadecimal();

private:
};