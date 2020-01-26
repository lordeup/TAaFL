#include "Lexer.h"

Lexer::Lexer(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_positionIndex = 0;
	m_lineNumber = 0;
	m_tempLineNumber = m_lineNumber;
	m_state = State::START;
	m_commentState = CommentState::NO_COMMENT;
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

void Lexer::Run()
{
	Initialize();

	while (std::getline(m_input, m_line))
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
					m_tokenVector.push_back(token);
				}
			}
		}
	}

	PrintTokenInfo();
}

Token Lexer::GetToken(std::string& str)
{
	Token token;
	SetState(State::START);

	token = CheckComment(str);

	if (m_commentState == CommentState::NO_COMMENT && token.type != TokenType::COMMENT)
	{
		size_t lenght = CheckOperation(str);
		bool isCheckOperation = lenght > 0;
		size_t lenghtWithBracket = 0;
		std::string substr;

		if (!isCheckOperation)
		{
			size_t foundPosition = str.find("(");

			if (foundPosition != std::string::npos && foundPosition != 0)
			{
				substr = str.substr(0, foundPosition);
				lenghtWithBracket = CheckOperation(substr);
			}
			else if (foundPosition == std::string::npos)
			{
				substr = str.substr(0, str.find("["));
				lenghtWithBracket = CheckOperation(substr);
			}
		}

		if (isCheckOperation || lenghtWithBracket > 0)
		{
			token = SetToken(TokenType::OPERATOR, isCheckOperation ? str : substr);
			str.erase(0, isCheckOperation ? lenght : lenghtWithBracket);
			SetState(State::FINISH);
		}
		else
		{
			char ch = str[0];

			if (m_state == State::START && IsFindVectorString(m_comparisonsOperator, str))
			{
				token = SetToken(TokenType::COMPARISONS_OPERATOR, str);
				str.erase(0, str.length());
				SetState(State::FINISH);
			}

			if (m_state == State::START && IsFindVectorString(m_logicalOperator, str))
			{
				token = SetToken(TokenType::LOGICAL_OPERATOR, str);
				str.erase(0, str.length());
				SetState(State::FINISH);
			}

			if (m_state == State::START && IsFindVectorString(m_bitwiseOperator, str))
			{
				token = SetToken(TokenType::BITWISE_OPERATOR, str);
				str.erase(0, str.length());
				SetState(State::FINISH);
			}

			if (m_state == State::START && ch == APOSTROPHE)
			{
				token = CheckChar(str);
			}

			if (m_state == State::START && ConvertCharToString(ch) == QUOTATION)
			{
				token = CheckString(str);
			}

			if (m_state == State::START && ch == '[')
			{
				token = CheckArray(str);
			}

			if (m_state == State::START && IsFindVectorChar(m_arithmeticOperator, ch) && !IsDigit(str[1]))
			{
				token = SetToken(TokenType::ARITHMETIC_OPERATOR, ConvertCharToString(ch));
				str.erase(0, 1);
				SetState(State::FINISH);
			}

			if (m_state == State::START && IsFindVectorChar(m_bracket, ch))
			{
				token = SetToken(TokenType::BRACKET, ConvertCharToString(ch));
				str.erase(0, 1);
				SetState(State::FINISH);
			}

			if (m_state == State::START && IsSeparator(ch))
			{
				token = SetToken(TokenType::SEPARATOR, ConvertCharToString(ch));
				str.erase(0, 1);
				SetState(State::FINISH);
			}

			if (m_state == State::START && IsDigit(ch))
			{
				token = CheckNumber(str);
			}

			if (m_state == State::START && IsLetter(ch))
			{
				token = CheckId(str);
			}
		}
	}

	if ((token.type == TokenType::ERROR && m_state == State::START) || token.type == TokenType::COMMENT)
	{
		token.value = str;
		str.erase(str.begin(), str.end());
	}

	token.lineNumber = m_lineNumber;

	return token;
}

size_t Lexer::CheckOperation(const std::string str) const
{
	auto it = std::find(m_operation.begin(), m_operation.end(), str);
	return it != m_operation.end() ? (*it).length() : 0;
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
				if (!IsFindVectorChar(m_binary, ch))
				{
					SetState(State::SKIP);
				}
				digits += ch;
			}
		}
		token.type = m_state == State::START ? TokenType::BINARY : TokenType::ERROR;
	}
	else if (numberSystem == "0o")
	{
		digits = numberSystem;
		for (const char& ch : substr)
		{
			if (!IsSeparator(ch))
			{
				if (!IsFindVectorChar(m_octal, ch))
				{
					SetState(State::SKIP);
				}
				digits += ch;
			}
		}
		token.type = m_state == State::START ? TokenType::OCTAL : TokenType::ERROR;
	}
	else if (numberSystem == "0x")
	{
		digits = numberSystem;
		for (const char& ch : substr)
		{
			if (!IsSeparator(ch))
			{
				if (!IsFindVectorChar(m_hexadecimal, ch))
				{
					SetState(State::SKIP);
				}
				digits += ch;
			}
		}
		token.type = m_state == State::START ? TokenType::HEXADECIMAL : TokenType::ERROR;
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

					if (IsSeparator(str[i + 2]))
					{
						SetState(State::SKIP);
					}
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

		if (token.type != TokenType::ERROR)
		{
			SetState(State::SKIP);
		}
	}

	token = SetToken(token.type, digits);

	if (token.type != TokenType::ERROR || m_state == State::SKIP)
	{
		SetState(State::FINISH);
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

	SetState(State::FINISH);
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
	SetState(State::FINISH);

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
	SetState(State::FINISH);

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
	SetState(State::FINISH);

	return token;
}

bool Lexer::IsSeparator(const char ch) const
{
	return IsFindVectorChar(m_separator, ch);
}

bool Lexer::IsFindVectorChar(const VectorChar& vec, const char ch) const
{
	return std::find(vec.begin(), vec.end(), ch) != vec.end();
}

bool Lexer::IsFindVectorString(const VectorString& vec, const std::string str) const
{
	return std::find(vec.begin(), vec.end(), str) != vec.end();
}

bool Lexer::IsDigit(const char ch) const
{
	return isdigit(ch) || ch == '-';
}

bool Lexer::IsLetter(const char ch) const
{
	return isalpha(ch) || ch == '_';
}

void Lexer::SetState(const State& state)
{
	m_state = state;
}

void Lexer::SetCommentState(const CommentState& commentState)
{
	m_commentState = commentState;
}

Token Lexer::SetToken(const TokenType& tokenType, const std::string str)
{
	return Token{ tokenType, str, m_line.find(str) };
}

std::string Lexer::ConvertCharToString(const char ch) const
{
	return std::string(1, ch);
}

void Lexer::PrintTokenInfo() const
{
	for (const auto& token : m_tokenVector)
	{
		m_output << token.value << SPACE << GetTokenType(token.type) << SPACE << token.lineNumber << SPACE << token.positionIndex << SPACE << std::endl;
	}
}

std::string Lexer::GetTokenType(const TokenType& tokenType) const
{
	switch (tokenType)
	{
	case TokenType::ERROR:
		return "ERROR";
	case TokenType::ID:
		return "ID";
	case TokenType::INTEGER:
		return "INTEGER";
	case TokenType::FLOAT:
		return "FLOAT";
	case TokenType::DOUBLE:
		return "DOUBLE";
	case TokenType::CHAR:
		return "CHAR";
	case TokenType::STRING:
		return "STRING";
	case TokenType::ARRAY:
		return "ARRAY";
	case TokenType::BINARY:
		return "BINARY";
	case TokenType::OCTAL:
		return "OCTAL";
	case TokenType::HEXADECIMAL:
		return "HEXADECIMAL";
	case TokenType::OPERATOR:
		return "OPERATOR";
	case TokenType::ARITHMETIC_OPERATOR:
		return "ARITHMETIC_OPERATOR";
	case TokenType::COMPARISONS_OPERATOR:
		return "COMPARISONS_OPERATOR";
	case TokenType::LOGICAL_OPERATOR:
		return "LOGICAL_OPERATOR";
	case TokenType::BITWISE_OPERATOR:
		return "BITWISE_OPERATOR";
	case TokenType::SEPARATOR:
		return "SEPARATOR";
	case TokenType::BRACKET:
		return "BRACKET";
	default:
		return "Unknown state";
	}
}
