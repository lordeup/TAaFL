#pragma once
#include <string>

namespace TokenType
{
	const std::string ERROR = "ERROR";
	const std::string ID = "ID";
	const std::string INTEGER = "INTEGER";
	const std::string FLOAT = "FLOAT";
	const std::string DOUBLE = "DOUBLE";
	const std::string CHAR = "CHAR";
	const std::string STRING = "STRING";
	const std::string ARRAY = "ARRAY";
	const std::string BINARY = "BINARY";
	const std::string OCTAL = "OCTAL";
	const std::string HEXADECIMAL = "HEXADECIMAL";

	const std::string OPERATOR_IF = "OPERATOR_IF";
	const std::string OPERATOR_ELSE = "OPERATOR_ELSE";
	const std::string OPERATOR_FOR = "OPERATOR_FOR";
	const std::string OPERATOR_WHILE = "OPERATOR_WHILE";
	const std::string OPERATOR_DO = "OPERATOR_DO";
	const std::string OPERATOR_BREAK = "OPERATOR_BREAK";
	const std::string OPERATOR_CONTINUE = "OPERATOR_CONTINUE";
	const std::string OPERATOR_RETURN = "OPERATOR_RETURN";
	const std::string OPERATOR_READ = "OPERATOR_READ";
	const std::string OPERATOR_WRITE = "OPERATOR_WRITE";
	const std::string OPERATOR_MAIN = "OPERATOR_MAIN";
	const std::string OPERATOR_VOID = "OPERATOR_VOID";
	const std::string OPERATOR_INT = "OPERATOR_INT";
	const std::string OPERATOR_FLOAT = "OPERATOR_FLOAT";
	const std::string OPERATOR_DOUBLE = "OPERATOR_DOUBLE";
	const std::string OPERATOR_CHAR = "OPERATOR_CHAR";
	const std::string OPERATOR_STRING = "OPERATOR_STRING";
	const std::string OPERATOR_BOOL = "OPERATOR_BOOL";

	const std::string PLUS = "PLUS";
	const std::string MINUS = "MINUS";
	const std::string MULTIPLICATION = "MULTIPLICATION";
	const std::string DIVISION = "DIVISION";
	const std::string ASSIGNMENT = "ASSIGNMENT";
	const std::string MOD = "MOD";

	const std::string LOGICAL_AND = "LOGICAL_AND";
	const std::string LOGICAL_OR = "LOGICAL_OR";
	const std::string LOGICAL_NOT = "LOGICAL_NOT";

	const std::string COMMA = "COMMA";
	const std::string SEMICOLON = "SEMICOLON";
	const std::string COLON = "COLON";

	const std::string BRACE_OPEN = "BRACE_OPEN";
	const std::string BRACE_CLOSE = "BRACE_CLOSE";
	const std::string BRACKET_OPEN = "BRACKET_OPEN";
	const std::string BRACKET_CLOSE = "BRACKET_CLOSE";

	const std::string BITWISE_AND = "BITWISE_AND";
	const std::string BITWISE_OR = "BITWISE_OR";
	const std::string BITWISE_NOT = "BITWISE_NOT";
	const std::string BITWISE_XOR = "BITWISE_XOR";
	const std::string BITWISE_RIGHT_SHIFT = "BITWISE_RIGHT_SHIFT";
	const std::string BITWISE_LEFT_SHIFT = "BITWISE_LEFT_SHIFT";

	const std::string MORE = "MORE";
	const std::string LESS = "LESS";
	const std::string EQUAL = "EQUAL";
	const std::string NOT_EQUAL = "NOT_EQUAL";
	const std::string MORE_OR_EQUAL = "MORE_OR_EQUAL";
	const std::string LESS_OR_EQUAL = "LESS_OR_EQUAL";

	const std::string COMMENT = "COMMENT";
};