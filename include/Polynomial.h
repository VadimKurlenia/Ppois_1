#pragma once

#include <iostream>
#include <stdexcept>

class Polynomial
{
private:
	int degree;
	double *coeffs;
public:
	void normalize();

	Polynomial(int deg);
	Polynomial(const double *arr, int deg);
	Polynomial(const Polynomial &polynomial);
	~Polynomial();

	double& operator[](int index) const;
	double operator()(double x) const;

	Polynomial operator+(const Polynomial &other) const;
	Polynomial& operator+=(const Polynomial &other);

	Polynomial operator-(const Polynomial &other) const;
	Polynomial& operator-=(const Polynomial &other);

	Polynomial operator*(const Polynomial &other) const;
	Polynomial& operator*=(const Polynomial &other);

	Polynomial operator/(const Polynomial &other) const;
	Polynomial& operator/=(const Polynomial &other);

	Polynomial& operator=(const Polynomial &polynomial);

	bool operator==(const Polynomial &other) const;
	bool operator!=(const Polynomial &other) const;

	friend std::ostream& operator<<(std::ostream &os, const Polynomial& poly);
	friend std::istream& operator>>(std::istream &is, Polynomial& poly);
};