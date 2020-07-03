#pragma once
#include "CommentState.h"
#include "Header.h"
#include "InitializeToken.h"
#include "State.h"
#include "TokenType.h"
#include <sstream>

struct Token
{
	TokenType type = TokenType::ERROR;
	std::string value;
	size_t positionIndex = 0;
	size_t lineNumber = 1;
};

class Lexer
{
public:
	Lexer();

	void Run();

	Token GetToken(std::string& str);

	std::string GetTokenType(TokenType& tokenType) const;

private:
	size_t m_lineNumber, m_tempLineNumber, m_positionIndex;
	std::string m_line;

	std::vector<Token> m_tokenVector;
	State m_state;
	CommentState m_commentState;

	VectorString m_comment, m_operation, m_comparisonsOperator, m_logicalOperator, m_bitwiseOperator;

	VectorChar m_arithmeticOperator, m_bracket, m_separator, m_binary, m_octal, m_decimalFractional, m_hexadecimal;

	void Initialize();

	size_t CheckOperation(const std::string str) const;
	Token CheckNumber(std::string& str);
	Token CheckId(std::string& str);
	Token CheckComment(const std::string str);
	Token CheckArray(std::string& str);
	Token CheckChar(std::string& str);
	Token CheckString(std::string& str);

	bool IsSeparator(const char ch) const;

	bool IsDigit(const char ch) const;
	bool IsLetter(const char ch) const;

	Token SetToken(const TokenType& tokenType, const std::string str);
	void SetState(const State& state);
	void SetCommentState(const CommentState& commentState);

	std::string ConvertCharToString(const char ch) const;
	bool IsFindVectorChar(const VectorChar& vec, const char ch) const;
	bool IsFindVectorString(const VectorString& vec, const std::string str) const;
};