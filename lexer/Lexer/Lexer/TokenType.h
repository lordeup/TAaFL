#pragma once

enum class TokenType
{
	ERROR,
	ID,
	INTEGER,
	FLOAT,
	DOUBLE,
	CHAR,
	STRING,
	ARRAY,
	BINARY,
	OCTAL,
	HEXADECIMAL,
	OPERATOR,
	ARITHMETIC_OPERATOR,
	COMPARISONS_OPERATOR,
	LOGICAL_OPERATOR,
	BITWISE_OPERATOR,
	SEPARATOR,
	BRACKET,
	COMMENT
};