#include "Set.h"
#include <sstream>
#include <algorithm>

SetElement::SetElement(const std::string &val) : type(Type::ATOM), atom(val), subset(nullptr) {}
SetElement::SetElement(const Set &s) : type(Type::SUBSET), subset(std::make_shared<Set>(s)) {}

bool SetElement::operator==(const SetElement &other) const
{
	if (type != other.type)
		return false;

	if(type == Type::ATOM)
	{
		return atom == other.atom;
	}
	else
	{
		return *subset == *other.subset;
	}
}

bool SetElement::operator!=(const SetElement &other) const
{
	return !(*this == other);
}

bool SetElement::operator<(const SetElement &other) const
{
	if(type != other.type)
	{
		return type < other.type;
	}

	if(type == Type::ATOM)
	{
		return atom < other.atom;
	}

	std::ostringstream ss1, ss2;

	ss1 << *subset;
	ss2 << *other.subset;

	return ss1.str() < ss2.str();
}

std::string Set::trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\n\r");

	if (first == std::string::npos)
		return "";

	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

void Set::sortElements()
{
	std::sort(elements.begin(), elements.end());
}

void Set::removeDuplicates()
{
	sortElements();
	auto it = std::unique(elements.begin(), elements.end());
	elements.erase(it, elements.end());
}

Set Set::parseString(const std::string &str)
{
	std::string s = trim(str);

	if (s.empty())
		return Set();

	if(s.front() == '{' && s.back() == '}')
	{
		s = s.substr(1, s.length() - 2);
	}

	Set result;
	int bracketCount = 0;
	std::string currentToken = "";

	for(size_t i = 0; i < s.length(); i++)
	{
		char ch = s[i];

		if (ch == '{')
		{
			bracketCount++;
			currentToken += ch;
		}
		else if(ch == '}')
		{
			bracketCount--;
			currentToken += ch;
		}
		else if(ch == ',' && bracketCount == 0)
		{
			std::string token = trim(currentToken);

			if(!token.empty())
			{
				if (token.front() == '{')
				{
					result.add(SetElement(parseString(token)));
				}
				else
				{
					result.add(SetElement(std::string(token)));
				}
			}

			currentToken = "";
		}
		else
		{
			currentToken += ch;
		}
	}

	std::string token = trim(currentToken);

	if(!token.empty())
	{
		if (token.front() == '{')
		{
			result.add(SetElement(parseString(token)));
		}
		else
		{
			result.add(SetElement(std::string(token)));
		}
	}

	return result;
}

Set::Set() {}

Set::Set(const char *str)
{
	*this = parseString(std::string(str));
}

Set::Set(const std::string &str)
{
	*this = parseString(str);
}

Set::Set(const Set& other) : elements(other.elements) {}

Set& Set::operator=(const Set& other)
{
	if(this != &other)
	{
		elements = other.elements;
	}

	return *this;
}

void Set::add(const SetElement& elem)
{
	elements.push_back(elem);
	removeDuplicates();
}

size_t Set::size() const
{
	return elements.size();
}

bool Set::empty() const
{
	return elements.empty();
}

bool Set::operator==(const Set& other) const
{
	if(elements.size() != other.elements.size())
		return false;

	for(size_t i = 0; i < elements.size(); i++)
	{
		if(elements[i] != other.elements[i])
			return false;
	}

	return true;
}

bool Set::operator!=(const Set& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Set& set)
{
	os << "{";

	for (size_t i = 0; i < set.elements.size(); ++i)
	{
		if(set.elements[i].type == SetElement::Type::ATOM)
		{
			os << set.elements[i].atom;
		}
		else
		{
			os << *set.elements[i].subset;
		}

		if(i + 1 < set.elements.size())
		{
			os << ", ";
		}
	}

	os << "}";
	return os;
}

std::istream& operator>>(std::istream& is, Set& set)
{
	std::string line;

	if(std::getline(is, line))
	{
		set = Set(line);
	}
	
	return is;
}