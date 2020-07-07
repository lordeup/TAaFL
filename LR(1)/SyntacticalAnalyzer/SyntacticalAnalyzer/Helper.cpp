#include "Helper.h"

std::string GetString(std::istringstream& iss)
{
	std::string str;
	iss >> str;
	return str;
}

int ParseNumber(const std::string str)
{
	try
	{
		return std::stoi(str);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Invalid argument: " + str);
	}
}

Symbol ParseSymbol(std::string str)
{
	State state;
	size_t number;

	if (str == "-")
	{
		state = State::None;
		number = 0;
	}
	else if (str == "Ok")
	{
		state = State::Ok;
		number = 0;
	}
	else
	{
		state = ParseState(str[0]);
		str.erase(0, 1);
		number = ParseNumber(str);
	}

	Symbol symbol = {
		state,
		number
	};

	return symbol;
}

State ParseState(char fCh)
{
	if (fCh == 'S')
	{
		return State::Shift;
	}
	else if (fCh == 'R')
	{
		return State::Convolution;
	}
	else
	{
		throw std::exception("Error, when trying parse state from char " + fCh);
	}
}