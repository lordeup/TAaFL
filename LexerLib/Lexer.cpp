#include "Lexer.h"

Lexer::Lexer()
{
	m_positionIndex = 0;
	m_lineNumber = 0;
	m_tempLineNumber = m_lineNumber;
	m_commentState = CommentState::NO_COMMENT;
	m_line = "";

	Initialize();
}

void Lexer::Initialize()
{
	InitializeToken initializeToken;
	m_comment = initializeToken.GetComment();
	m_operation = initializeToken.GetOperator();
	m_arithmeticOperator = initializeToken.GetArithmeticOperator();
	m_comparisonsOperator = initializeToken.GetComparisonsOperator();
	m_logicalOperator = initializeToken.GetLogicalOperator();
	m_bitwiseOperator = initializeToken.GetBitwiseOperator();
	m_separator = initializeToken.GetSeparator();
	m_bracket = initializeToken.GetBracket();
	m_binary = initializeToken.GetBinary();
	m_octal = initializeToken.GetOctal();
	m_decimalFractional = initializeToken.GetDecimalFractional();
	m_hexadecimal = initializeToken.GetHexadecimal();
}

std::vector<Token> Lexer::GetTokenDatas(std::istream& input)
{
	while (std::getline(input, m_line))
	{
		++m_lineNumber;

		std::istringstream iss(m_line);
		std::string str;

		while (iss >> str)
		{
			while (!str.empty())
			{
				Token token = GetToken(str);
				if (token.type != TokenType::COMMENT)
				{
					m_tokens.push_back(token);
				}
			}
		}
	}

	return m_tokens;
}

Token Lexer::GetToken(std::string& str)
{
	Token token = CheckComment(str);

	if (m_commentState == CommentState::NO_COMMENT && token.type != TokenType::COMMENT)
	{
		auto operation = GetFindVectorString(m_operation, str);
		size_t lenght = operation != m_operation.end() ? (*operation).value.length() : 0;
		bool isCheckOperation = lenght > 0;
		size_t lenghtWithBracket = 0;
		std::string substr;

		if (!isCheckOperation)
		{
			size_t foundPosition = str.find("(");

			if (foundPosition != std::string::npos && foundPosition != 0)
			{
				substr = str.substr(0, foundPosition);
				auto it = GetFindVectorString(m_operation, substr);
				lenghtWithBracket = it != m_operation.end() ? (*it).value.length() : 0;
			}
			else if (foundPosition == std::string::npos)
			{
				substr = str.substr(0, str.find("["));
				auto it = GetFindVectorString(m_operation, substr);
				lenghtWithBracket = it != m_operation.end() ? (*it).value.length() : 0;
			}
		}

		if (isCheckOperation || lenghtWithBracket > 0)
		{
			token = SetToken((*operation).type, isCheckOperation ? str : substr);
			str.erase(0, isCheckOperation ? lenght : lenghtWithBracket);
			return token;
		}
		else
		{
			char ch = str[0];

			auto comparison = GetFindVectorString(m_comparisonsOperator, str);

			if (comparison != m_comparisonsOperator.end())
			{
				token = SetToken((*comparison).type, str);
				str.erase(0, str.length());
				return token;
			}

			auto logical = GetFindVectorString(m_logicalOperator, str);

			if (logical != m_logicalOperator.end())
			{
				token = SetToken((*logical).type, str);
				str.erase(0, str.length());
				return token;
			}

			auto bitwise = GetFindVectorString(m_bitwiseOperator, str);

			if (bitwise != m_bitwiseOperator.end())
			{
				token = SetToken((*bitwise).type, str);
				str.erase(0, str.length());
				return token;
			}

			if (ch == APOSTROPHE)
			{
				return CheckChar(str);
			}

			if (ConvertCharToString(ch) == QUOTATION)
			{
				return CheckString(str);
			}

			if (ch == '[')
			{
				return CheckArray(str);
			}

			auto arithmetic = GetFindVectorChar(m_arithmeticOperator, ch);

			if (arithmetic != m_arithmeticOperator.end() && !IsDigit(str[1]))
			{
				token = SetToken((*arithmetic).type, ConvertCharToString(ch));
				str.erase(0, 1);
				return token;
			}

			auto bracket = GetFindVectorChar(m_bracket, ch);

			if (bracket != m_bracket.end())
			{
				token = SetToken((*bracket).type, ConvertCharToString(ch));
				str.erase(0, 1);
				return token;
			}

			auto separator = GetFindVectorChar(m_separator, ch);

			if (separator != m_separator.end())
			{
				token = SetToken((*separator).type, ConvertCharToString(ch));
				str.erase(0, 1);
				return token;
			}

			if (IsDigit(ch))
			{
				return CheckNumber(str);
			}

			if (IsLetter(ch))
			{
				return CheckId(str);
			}
		}
	}

	if (token.type == TokenType::ERROR || token.type == TokenType::COMMENT)
	{
		token.value = str;
		str.erase(str.begin(), str.end());
	}

	return token;
}

Token Lexer::CheckNumber(std::string& str)
{
	Token token;
	std::string digits;

	std::string numberSystem = str.substr(0, 2);
	std::string substr = str.length() >= 2 ? str.substr(2) : str;

	if (numberSystem == "0b")
	{
		digits = numberSystem;
		for (const char& ch : substr)
		{
			if (!IsSeparator(ch))
			{
				digits += ch;
			}
		}
		token.type = TokenType::BINARY;
	}
	else if (numberSystem == "0o")
	{
		digits = numberSystem;
		for (const char& ch : substr)
		{
			if (!IsSeparator(ch))
			{
				digits += ch;
			}
		}
		token.type = TokenType::OCTAL;
	}
	else if (numberSystem == "0x")
	{
		digits = numberSystem;
		for (const char& ch : substr)
		{
			if (!IsSeparator(ch))
			{
				digits += ch;
			}
		}
		token.type = TokenType::HEXADECIMAL;
	}
	else
	{
		for (size_t i = 0; i < str.length(); ++i)
		{
			char ch = str[i];
			char nextCh = str[i + 1];
			bool isDecimalFractional = IsFindVectorChar(m_decimalFractional, ch);
			bool isNextCh = IsDigit(nextCh) || nextCh == MINUS || nextCh == PLUS;

			if (!IsSeparator(ch) && (IsDigit(ch) || isDecimalFractional || ch == PLUS))
			{
				if (ch == 'E' && isNextCh)
				{
					token.type = TokenType::DOUBLE;
				}

				if (IsDigit(ch) && nextCh == POINT)
				{
					token.type = isdigit(str[i + 2]) ? TokenType::FLOAT : TokenType::ERROR;
				}

				if (token.type != TokenType::FLOAT && token.type != TokenType::DOUBLE)
				{
					token.type = isDecimalFractional && !isNextCh ? TokenType::ERROR : TokenType::INTEGER;
				}
				digits += ch;
			}
			else if (IsLetter(ch))
			{
				token.type = TokenType::ERROR;
				break;
			}
		}
	}

	token = SetToken(token.type, digits);

	if (token.type != TokenType::ERROR)
	{
		str.erase(0, digits.length());
	}

	return token;
}

Token Lexer::CheckId(std::string& str)
{
	Token token;
	std::string letters;
	for (const char& ch : str)
	{
		if (IsLetter(ch) || isdigit(ch))
		{
			letters += ch;
		}
	}

	token = SetToken(letters.length() <= 128 ? TokenType::ID : TokenType::ERROR, letters);
	str.erase(0, letters.length());

	return token;
}

Token Lexer::CheckComment(const std::string str)
{
	Token token;
	std::string str1;

	for (size_t i = 0; i < str.length(); ++i)
	{
		char ch = str[i];
		char nextCh = str[i + 1];
		if ((ch == SLASH || ch == STAR) && (nextCh == SLASH || nextCh == STAR))
		{
			str1 = ConvertCharToString(ch) + ConvertCharToString(nextCh);
			break;
		}
	}

	if (IsFindVectorString(m_comment, str1) || m_commentState != CommentState::NO_COMMENT)
	{
		if (str1 == "//" && m_commentState != CommentState::MULTILINE)
		{
			SetCommentState(CommentState::SINGLE_LINE);
			m_tempLineNumber = m_lineNumber;
		}
		else if (str1 == "/*")
		{
			SetCommentState(CommentState::MULTILINE);
		}
		else if (str1 == "*/")
		{
			SetCommentState(CommentState::NO_COMMENT);
		}

		if (m_commentState == CommentState::SINGLE_LINE && m_tempLineNumber != m_lineNumber)
		{
			SetCommentState(CommentState::NO_COMMENT);
		}

		token.type = TokenType::COMMENT;
	}

	return token;
}

Token Lexer::CheckArray(std::string& str)
{
	Token token;
	std::string strArray;
	for (const char& ch : str)
	{
		if (!IsSeparator(ch))
		{
			strArray += ch;
		}
	}

	if (strArray[0] == '[' && strArray[strArray.length() - 1] == ']')
	{
		token.type = TokenType::ARRAY;
	}
	token = SetToken(token.type, strArray);
	str.erase(0, strArray.length());

	return token;
}

Token Lexer::CheckChar(std::string& str)
{
	Token token;
	std::string strChar;
	for (const char& ch : str)
	{
		if (!IsSeparator(ch))
		{
			strChar += ch;
		}
	}

	if (strChar[0] == APOSTROPHE && strChar[strChar.length() - 1] == APOSTROPHE && strChar.length() >= 2)
	{
		token.type = TokenType::CHAR;
	}
	token = SetToken(token.type, strChar);
	str.erase(0, strChar.length());

	return token;
}

Token Lexer::CheckString(std::string& str)
{
	Token token;
	std::string str1;
	for (const char& ch : str)
	{
		if (!IsSeparator(ch))
		{
			str1 += ch;
		}
	}

	if (ConvertCharToString(str1[0]) == QUOTATION && ConvertCharToString(str1[str1.length() - 1]) == QUOTATION && str1.length() >= 2)
	{
		token.type = TokenType::STRING;
	}
	token = SetToken(token.type, str1);
	str.erase(0, str1.length());

	return token;
}

bool Lexer::IsSeparator(const char ch) const
{
	return GetFindVectorChar(m_separator, ch) != m_separator.end();
}

bool Lexer::IsFindVectorChar(const VectorChar& vec, const char ch) const
{
	return std::find(vec.begin(), vec.end(), ch) != vec.end();
}

bool Lexer::IsFindVectorString(const VectorString& vec, const std::string str) const
{
	return std::find(vec.begin(), vec.end(), str) != vec.end();
}

std::vector<TokenDataChar>::const_iterator Lexer::GetFindVectorChar(const std::vector<TokenDataChar>& vec, const char ch) const
{
	return std::find_if(vec.begin(), vec.end(), [&](const TokenDataChar& data) { return data.value == ch; });
}

std::vector<TokenDataString>::const_iterator Lexer::GetFindVectorString(const std::vector<TokenDataString>& vec, const std::string str) const
{
	return std::find_if(vec.begin(), vec.end(), [&](const TokenDataString& data) { return data.value == str; });
}

bool Lexer::IsDigit(const char ch) const
{
	return isdigit(ch) || ch == '-';
}

bool Lexer::IsLetter(const char ch) const
{
	return isalpha(ch) || ch == '_';
}

void Lexer::SetCommentState(const CommentState& commentState)
{
	m_commentState = commentState;
}

Token Lexer::SetToken(const std::string& tokenType, const std::string str)
{
	return Token{ tokenType, str, m_line.find(str), m_lineNumber };
}

std::string Lexer::ConvertCharToString(const char ch) const
{
	return std::string(1, ch);
}
