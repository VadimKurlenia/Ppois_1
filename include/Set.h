#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Set;

class SetElement
{
public:
	enum class Type
	{
		ATOM, SUBSET
	};

	Type type;
	std::string atom;
	std::shared_ptr<Set> subset;

	SetElement(const std::string &val);
	SetElement(const Set& s);

	bool operator==(const SetElement& other) const;
	bool operator!=(const SetElement& other) const;
	bool operator<(const SetElement& other) const;
};

class Set
{
private:
	std::vector<SetElement> elements;

	void removeDuplicates();
	void sortElements();

	static std::string trim(const std::string &str);
	static Set parseString(const std::string &str);

public:
	Set();
	Set(const char* str);
	Set(const std::string& str);
	Set(const Set& other);
	Set& operator=(const Set& other);
	~Set() = default;

	void add(const SetElement& elem);
	size_t size() const;
	bool empty() const;

	bool operator==(const Set& other) const;
	bool operator!=(const Set& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Set& set);
	friend std::istream& operator>>(std::istream& is, Set& set);
};