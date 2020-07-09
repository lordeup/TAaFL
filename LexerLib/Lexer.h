#pragma once
#include "CommentState.h"
#include "Header.h"
#include "InitializeToken.h"
#include "TokenType.h"
#include <sstream>

struct Token
{
	std::string type = TokenType::ERROR;
	std::string value;
	size_t positionIndex = 0;
	size_t lineNumber = 1;
};

class Lexer
{
public:
	Lexer();

	std::vector<Token> GetTokenDatas(std::istream& input);
	Token GetToken(std::string& str);

private:
	size_t m_lineNumber, m_tempLineNumber, m_positionIndex;
	std::string m_line;

	std::vector<Token> m_tokens;
	CommentState m_commentState;

	VectorString m_comment;
	std::vector<TokenDataString> m_operation, m_comparisonsOperator, m_logicalOperator, m_bitwiseOperator;
	std::vector<TokenDataChar> m_arithmeticOperator, m_bracket, m_separator;

	VectorChar m_binary, m_octal, m_decimalFractional, m_hexadecimal;

	void Initialize();

	Token CheckNumber(std::string& str);
	Token CheckId(std::string& str);
	Token CheckComment(const std::string str);
	Token CheckArray(std::string& str);
	Token CheckChar(std::string& str);
	Token CheckString(std::string& str);

	bool IsSeparator(const char ch) const;

	bool IsDigit(const char ch) const;
	bool IsLetter(const char ch) const;

	Token SetToken(const std::string& tokenType, const std::string str);
	void SetCommentState(const CommentState& commentState);

	std::string ConvertCharToString(const char ch) const;
	bool IsFindVectorChar(const VectorChar& vec, const char ch) const;
	bool IsFindVectorString(const VectorString& vec, const std::string str) const;

	std::vector<TokenDataChar>::const_iterator GetFindVectorChar(const std::vector<TokenDataChar>& vec, const char ch) const;
	std::vector<TokenDataString>::const_iterator GetFindVectorString(const std::vector<TokenDataString>& vec, const std::string str) const;
};