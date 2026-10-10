#include "Polynomial.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

void Polynomial::normalize()
{
	while (degree > 0 && std::abs(coeffs[degree]) == 0.0)
	{
		degree--;
	}
}

Polynomial::Polynomial(int deg)
{
	if (deg < 0)
	{
		deg = 0;
	}

	degree = deg;
	coeffs = new double[degree + 1]();
}

Polynomial::Polynomial(const double *arr, int deg)
{
	if (deg < 0)
	{
		deg = 0;
	}

	degree = deg;
	coeffs = new double[degree + 1];

	for (int i = 0; i <= degree; i++)
	{
		coeffs[i] = arr[i];
	}

	normalize();
}

Polynomial::Polynomial(const Polynomial &polynomial) : degree(polynomial.degree)
{
	coeffs = new double[degree + 1];

	for(int i = 0; i <= degree; ++i)
	{
		coeffs[i] = polynomial.coeffs[i];
	}
}

Polynomial::~Polynomial()
{
	delete[] coeffs;
}

double& Polynomial::operator[](int index) const
{
	if (index < 0 || index > degree)
	{
		throw std::out_of_range("Выход за пределы допустимого индекса!");
	}

	return coeffs[index];
}

double Polynomial::operator()(double x) const
{
	double result = 0.0;

	for (int i = degree; i >= 0; i--)
	{
		result = result * x + coeffs[i];
	}

	return result;
}

Polynomial Polynomial::operator+(const Polynomial &other) const
{
	int maxDegree = std::max(degree, other.degree);
	Polynomial result (maxDegree);

	for (int i = 0; i <= maxDegree; i++)
	{
		double c1 = (i <= degree) ? coeffs[i] : 0.0;
		double c2 = (i <= other.degree) ? other.coeffs[i] : 0.0;

		result.coeffs[i] = c1 + c2;
	}

	result.normalize();
	return result;
}

Polynomial& Polynomial::operator+=(const Polynomial &other)
{
	*this = *this + other;
	return *this;
}

Polynomial Polynomial::operator-(const Polynomial &other) const
{
	int maxDegree = std::max(degree, other.degree);
	Polynomial result { maxDegree };

	for (int i = 0; i <= maxDegree; i++)
	{
		double c1 = (i <= degree) ? coeffs[i] : 0.0;
		double c2 = (i <= other.degree) ? other.coeffs[i] : 0.0;

		result.coeffs[i] = c1 - c2;
	}

	result.normalize();
	return result;
}

Polynomial& Polynomial::operator-=(const Polynomial &other)
{
	*this = *this - other;
	return *this;
}

Polynomial Polynomial::operator*(const Polynomial &other) const
{
	if ((degree == 0 && this->coeffs[0] == 0) || (other.degree == 0 && other.coeffs[0] == 0))
	{
		return Polynomial(0);
	}

	int maxDegree = degree + other.degree;
	Polynomial result { maxDegree };

	for (int i = 0; i <= degree; i++)
	{
		for (int j = 0; j <= other.degree; j++)
		{
			result.coeffs[i + j] += coeffs[i] * other.coeffs[j];
		}
	}

	result.normalize();
	return result;
}

Polynomial& Polynomial::operator*=(const Polynomial &other)
{
	*this = *this * other;
	return *this;
}

Polynomial Polynomial::operator/(const Polynomial &other) const
{
	if(other.degree == 0 && other.coeffs[0] == 0)
	{
		throw std::invalid_argument("Деление на 0 запрещено!");
	}

	if (degree < other.degree)
	{
		return Polynomial(0);
	}

	Polynomial q(degree - other.degree);

	for(int i = 0; i <= q.degree; i++)
	{
		q.coeffs[i] = 0.0;
	}

	Polynomial rem = *this;

	while (rem.degree >= other.degree && !(rem.degree == 0 && std::abs(rem.coeffs[0]) == 0))
	{
		int degDiff = rem.degree - other.degree;
		double factor = rem.coeffs[rem.degree] / other.coeffs[other.degree];
		
		q.coeffs[degDiff] = factor;
		Polynomial term(degDiff);

		for (int i = 0; i < degDiff; i++)
		{
			term.coeffs[i] = 0;
		}

		term.coeffs[degDiff] = factor;

		Polynomial sub = term * other;
		sub.coeffs[sub.degree] = rem.coeffs[rem.degree];
		rem = rem - sub;
	}

	q.normalize();
	return q;
}

Polynomial& Polynomial::operator/=(const Polynomial &other)
{
	*this = *this / other;
	return *this;
}

Polynomial& Polynomial::operator=(const Polynomial &other)
{
	if (this != &other)
	{
		delete[] coeffs;

		degree = other.degree;
		coeffs = new double[degree + 1];

		for (int i = 0; i <= degree; i++)
		{
			coeffs[i] = other.coeffs[i];
		}
	}

	return *this;
}

bool Polynomial::operator==(const Polynomial &other) const
{
	if (degree == other.degree)
	{
		for (int i = 0; i <= degree; i++)
		{
			if (coeffs[i] != other.coeffs[i])
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool Polynomial::operator!=(const Polynomial &other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream &os, const Polynomial& poly)
{
	bool first = true;

	for (int i = poly.degree; i >= 0; i--)
	{
		double coeff = poly.coeffs[i];

		if (std::abs(coeff) == 0)
		{
			continue;
		}

		if (!first && coeff > 0) os << " + ";
		if (coeff < 0) os << (first ? "-" : " - ");
		
		if (i == 0 || std::abs(coeff) != 1.0)
		{
			os << std::abs(coeff);
		}

		if(i > 0)
		{
			os << "x";

			if (i > 1)
				os << "^" << i;
		}

		first = false;
	}

	if (first)
		os << "0";
	
	return os;
}

std::istream& operator>>(std::istream &is, Polynomial& poly)
{
	int deg;
	
	if(!(is >> deg) || deg < 0)
		return is;

	delete[] poly.coeffs;

	poly.degree = deg;
	poly.coeffs = new double[deg + 1]();

	for(int i = 0; i <= deg; i++)
	{
		is >> poly.coeffs[i];
	}

	poly.normalize();
	return is;
}