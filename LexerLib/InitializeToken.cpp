#include "InitializeToken.h"

VectorString InitializeToken::GetComment()
{
	return VectorString{ "//", "/*", "*/" };
}

std::vector<TokenDataString> InitializeToken::GetOperator()
{
	std::vector<TokenDataString> vec;
	vec.push_back({ TokenType::OPERATOR_IF, "if" });
	vec.push_back({ TokenType::OPERATOR_ELSE, "else" });
	vec.push_back({ TokenType::OPERATOR_FOR, "for" });
	vec.push_back({ TokenType::OPERATOR_WHILE, "while" });
	vec.push_back({ TokenType::OPERATOR_DO, "do" });
	vec.push_back({ TokenType::OPERATOR_BREAK, "break" });
	vec.push_back({ TokenType::OPERATOR_CONTINUE, "continue" });
	vec.push_back({ TokenType::OPERATOR_RETURN, "return" });
	vec.push_back({ TokenType::OPERATOR_READ, "read" });
	vec.push_back({ TokenType::OPERATOR_WRITE, "write" });
	vec.push_back({ TokenType::OPERATOR_MAIN, "main" });
	vec.push_back({ TokenType::OPERATOR_VOID, "void" });
	vec.push_back({ TokenType::OPERATOR_INT, "int" });
	vec.push_back({ TokenType::OPERATOR_FLOAT, "float" });
	vec.push_back({ TokenType::OPERATOR_DOUBLE, "double" });
	vec.push_back({ TokenType::OPERATOR_CHAR, "char" });
	vec.push_back({ TokenType::OPERATOR_STRING, "string" });
	vec.push_back({ TokenType::OPERATOR_BOOL, "bool" });

	return vec;
}

std::vector<TokenDataString> InitializeToken::GetLogicalOperator()
{
	std::vector<TokenDataString> vec;
	vec.push_back({ TokenType::LOGICAL_AND, "&&" });
	vec.push_back({ TokenType::LOGICAL_OR, "||" });
	vec.push_back({ TokenType::LOGICAL_NOT, "!" });

	return vec;
}

std::vector<TokenDataString> InitializeToken::GetBitwiseOperator()
{
	std::vector<TokenDataString> vec;
	vec.push_back({ TokenType::BITWISE_AND, "&" });
	vec.push_back({ TokenType::BITWISE_OR, "|" });
	vec.push_back({ TokenType::BITWISE_NOT, "~" });
	vec.push_back({ TokenType::BITWISE_XOR, "^" });
	vec.push_back({ TokenType::BITWISE_LEFT_SHIFT, "<<" });
	vec.push_back({ TokenType::BITWISE_RIGHT_SHIFT, ">>" });

	return vec;
}

std::vector<TokenDataChar> InitializeToken::GetArithmeticOperator()
{
	std::vector<TokenDataChar> vec;
	vec.push_back({ TokenType::PLUS, '+' });
	vec.push_back({ TokenType::MINUS, '-' });
	vec.push_back({ TokenType::MULTIPLICATION, '*' });
	vec.push_back({ TokenType::DIVISION, '/' });
	vec.push_back({ TokenType::ASSIGNMENT, '=' });
	vec.push_back({ TokenType::MOD, '%' });

	return vec;
}

std::vector<TokenDataString> InitializeToken::GetComparisonsOperator()
{
	std::vector<TokenDataString> vec;
	vec.push_back({ TokenType::MORE, ">" });
	vec.push_back({ TokenType::LESS, "<" });
	vec.push_back({ TokenType::EQUAL, "==" });
	vec.push_back({ TokenType::NOT_EQUAL, "!=" });
	vec.push_back({ TokenType::MORE_OR_EQUAL, ">=" });
	vec.push_back({ TokenType::LESS_OR_EQUAL, "<=" });

	return vec;
}

std::vector<TokenDataChar> InitializeToken::GetBracket()
{
	std::vector<TokenDataChar> vec;
	vec.push_back({ TokenType::BRACE_OPEN, '{' });
	vec.push_back({ TokenType::BRACE_CLOSE, '}' });
	vec.push_back({ TokenType::BRACKET_OPEN, '(' });
	vec.push_back({ TokenType::BRACKET_CLOSE, ')' });

	return vec;
}

std::vector<TokenDataChar> InitializeToken::GetSeparator()
{
	std::vector<TokenDataChar> vec;
	vec.push_back({ TokenType::COMMA, ',' });
	vec.push_back({ TokenType::SEMICOLON, ';' });
	vec.push_back({ TokenType::COLON, ':' });

	return vec;
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
