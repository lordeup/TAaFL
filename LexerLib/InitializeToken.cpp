#include "InitializeToken.h"

InitializeToken::InitializeToken()
{
}

VectorString InitializeToken::GetComment()
{
	return VectorString{ "//", "/*", "*/" };
}

VectorString InitializeToken::GetOperator()
{
	return VectorString{ "if", "else", "for", "while", "do", "break", "continue", "return", "read", "write", "main", "void", "int", "float", "double", "char", "string", "bool" };
}

VectorString InitializeToken::GetLogicalOperator()
{
	return VectorString{ "&&", "||", "!" };
}

VectorString InitializeToken::GetBitwiseOperator()
{
	return VectorString{ "~", "&", "|", "^", "<<", ">>" };
}

VectorChar InitializeToken::GetArithmeticOperator()
{
	return VectorChar{ '+', '-', '*', '/', '=', '%' };
}

VectorString InitializeToken::GetComparisonsOperator()
{
	return VectorString{ "==", "!=", ">", "<", ">=", "<=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=" };
}

VectorChar InitializeToken::GetBracket()
{
	return VectorChar{ '{', '}', '(', ')' };
}

VectorChar InitializeToken::GetSeparator()
{
	return VectorChar{ ',', ';', ':' };
}

VectorChar InitializeToken::GetBinary()
{
	return VectorChar{ '0', '1' };
}

VectorChar InitializeToken::GetOctal()
{
	return VectorChar{ '0', '1', '2', '3', '4', '5', '6', '7' };
}

VectorChar InitializeToken::GetDecimalFractional()
{
	return VectorChar{ 'E', '.' };
}

VectorChar InitializeToken::GetHexadecimal()
{
	return VectorChar{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
}
