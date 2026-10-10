#include "ut.h"
#include "Polynomial.h"
#include <sstream>
#include <vector>
#include <initializer_list>
#include <stdexcept>

namespace
{
	const double EPS = 1e-9;

	// make({1,2,3}) -> 1 + 2x + 3x^2 (коэффициенты от младшего к старшему)
	Polynomial make(std::initializer_list<double> c)
	{
		std::vector<double> v(c);
		return Polynomial(v.data(), static_cast<int>(v.size()) - 1);
	}

	std::string str(const Polynomial &p)
	{
		std::ostringstream os;
		os << p;
		return os.str();
	}
}

SUITE(PolynomialTests)
{
	// ---------- Конструкторы ----------
	TEST(ZeroDegreeConstructorGivesZeroPolynomial)
	{
		Polynomial p(0);
		CHECK_STR("0", str(p));
		CHECK_CLOSE(0.0, p[0], EPS);
	}

	TEST(NegativeDegreeBecomesZero)
	{
		Polynomial p(-3);
		CHECK_THROW(p[1], std::out_of_range);
		CHECK_STR("0", str(p));
	}

	TEST(ConstructorFromArrayCopiesAllCoefficients)
	{
		Polynomial p = make({1, 2, 3});
		CHECK_CLOSE(1.0, p[0], EPS);
		CHECK_CLOSE(2.0, p[1], EPS);
		CHECK_CLOSE(3.0, p[2], EPS);
	}

	TEST(ConstructorFromArrayRemovesLeadingZeros)
	{
		Polynomial p = make({1, 2, 0, 0});
		CHECK_THROW(p[2], std::out_of_range);
		CHECK_STR("2x + 1", str(p));
	}

	TEST(ConstructorFromArrayAllZerosGivesZero)
	{
		Polynomial p = make({0, 0, 0});
		CHECK_THROW(p[1], std::out_of_range);
		CHECK_STR("0", str(p));
	}

	TEST(CopyConstructorCopiesCoefficients)
	{
		Polynomial a = make({1, 2, 3});
		Polynomial b(a);
		CHECK(a == b);
	}

	TEST(CopyIsIndependentFromOriginal)
	{
		Polynomial a = make({1, 2});
		Polynomial b(a);
		b[0] = 100;
		CHECK_CLOSE(1.0, a[0], EPS);
		CHECK_CLOSE(100.0, b[0], EPS);
	}

	// ---------- operator[] ----------
	TEST(IndexNegativeThrows)
	{
		Polynomial p = make({1, 2, 3});
		CHECK_THROW(p[-1], std::out_of_range);
	}

	TEST(IndexAboveDegreeThrows)
	{
		Polynomial p = make({1, 2, 3});
		CHECK_THROW(p[3], std::out_of_range);
	}

	TEST(IndexAssignmentChangesCoefficient)
	{
		Polynomial p = make({1, 2, 3});
		p[1] = 10;
		CHECK_CLOSE(10.0, p[1], EPS);
	}

	TEST(IndexWorksOnConstObject)
	{
		const Polynomial p = make({4, 5});
		CHECK_CLOSE(5.0, p[1], EPS);
	}

	// ---------- Вычисление значения ----------
	TEST(EvaluateAtPoint)
	{
		CHECK_CLOSE(17.0, make({1, 2, 3})(2.0), EPS);
	}

	TEST(EvaluateAtZeroGivesFreeTerm)
	{
		CHECK_CLOSE(5.0, make({5, 3})(0.0), EPS);
	}

	TEST(EvaluateAtNegativePoint)
	{
		CHECK_CLOSE(2.0, make({1, 2, 3})(-1.0), EPS);
	}

	TEST(EvaluateConstant)
	{
		CHECK_CLOSE(7.0, make({7})(100.0), EPS);
	}

	TEST(EvaluateZeroPolynomial)
	{
		CHECK_CLOSE(0.0, Polynomial(0)(42.0), EPS);
	}

	// ---------- Сложение ----------
	TEST(AddSameDegree)
	{
		CHECK(make({5, 7, 9}) == make({1, 2, 3}) + make({4, 5, 6}));
	}

	TEST(AddDifferentDegreeFirstShorter)
	{
		CHECK(make({2, 4, 3}) == make({1, 2}) + make({1, 2, 3}));
	}

	TEST(AddDifferentDegreeSecondShorter)
	{
		CHECK(make({2, 4, 3}) == make({1, 2, 3}) + make({1, 2}));
	}

	TEST(AddCancelingLeadingTermReducesDegree)
	{
		Polynomial r = make({1, 2, 3}) + make({0, 0, -3});
		CHECK_THROW(r[2], std::out_of_range);
		CHECK_STR("2x + 1", str(r));
	}

	TEST(AddOppositeGivesZero)
	{
		CHECK_STR("0", str(make({1, 2, 3}) + make({-1, -2, -3})));
	}

	TEST(PlusAssign)
	{
		Polynomial a = make({1, 2});
		a += make({1, 1, 1});
		CHECK(make({2, 3, 1}) == a);
	}

	// ---------- Вычитание ----------
	TEST(SubtractSameDegree)
	{
		CHECK(make({1, 2, 3}) == make({5, 7, 9}) - make({4, 5, 6}));
	}

	TEST(SubtractShorterFromLonger)
	{
		CHECK_STR("3x^2", str(make({1, 2, 3}) - make({1, 2})));
	}

	TEST(SubtractLongerFromShorter)
	{
		CHECK_STR("-3x^2", str(make({1, 2}) - make({1, 2, 3})));
	}

	TEST(SubtractSelfGivesZero)
	{
		Polynomial a = make({1, 2, 3});
		CHECK_STR("0", str(a - a));
	}

	TEST(MinusAssign)
	{
		Polynomial a = make({5, 5, 5});
		a -= make({1, 2, 3});
		CHECK(make({4, 3, 2}) == a);
	}

	// ---------- Умножение ----------
	TEST(MultiplyBinomials)
	{
		// (1 + x)(1 - x) = 1 - x^2
		CHECK_STR("-x^2 + 1", str(make({1, 1}) * make({1, -1})));
	}

	TEST(MultiplyByZeroGivesZero)
	{
		CHECK_STR("0", str(make({1, 2, 3}) * Polynomial(0)));
		CHECK_STR("0", str(Polynomial(0) * make({1, 2, 3})));
	}

	TEST(MultiplyByConstant)
	{
		CHECK(make({2, 4, 6}) == make({1, 2, 3}) * make({2}));
	}

	TEST(MultiplyIncreasesDegree)
	{
		Polynomial x = make({0, 1});
		CHECK_STR("x^3", str(x * x * x));
	}

	TEST(MultiplyIsCommutative)
	{
		Polynomial a = make({1, 2, 3}), b = make({4, 0, 5, 6});
		CHECK(a * b == b * a);
	}

	TEST(MultiplyAssign)
	{
		Polynomial a = make({1, 1});
		a *= make({1, 1});
		CHECK(make({1, 2, 1}) == a);
	}

	// ---------- Деление ----------
	TEST(DivideExactly)
	{
		// (x^2 - 1) / (x - 1) = x + 1
		CHECK_STR("x + 1", str(make({-1, 0, 1}) / make({-1, 1})));
	}

	TEST(DivideWithRemainderGivesQuotient)
	{
		// (x^2 + 1) / x = x (остаток 1 отбрасывается)
		CHECK_STR("x", str(make({1, 0, 1}) / make({0, 1})));
	}

	TEST(DivideCubicByLinear)
	{
		// (x^3 - 6x^2 + 11x - 6) / (x - 1) = x^2 - 5x + 6
		CHECK_STR("x^2 - 5x + 6", str(make({-6, 11, -6, 1}) / make({-1, 1})));
	}

	TEST(DivideByHigherDegreeGivesZero)
	{
		CHECK_STR("0", str(make({1, 1}) / make({1, 0, 1})));
	}

	TEST(DivideByZeroPolynomialThrows)
	{
		CHECK_THROW(make({1, 2}) / Polynomial(0), std::invalid_argument);
	}

	TEST(DivideByConstant)
	{
		CHECK(make({1, 2, 3}) == make({2, 4, 6}) / make({2}));
	}

	TEST(DivideAssign)
	{
		Polynomial a = make({-1, 0, 1});
		a /= make({-1, 1});
		CHECK(make({1, 1}) == a);
	}

	TEST(MultiplyThenDivideReturnsOriginal)
	{
		Polynomial q = make({6, -5, 1}), d = make({-1, 1});
		CHECK(q == (q * d) / d);
	}

	// ---------- Сравнение ----------
	TEST(EqualPolynomialsAreEqual)
	{
		CHECK(make({1, 2, 3}) == make({1, 2, 3}));
	}

	TEST(DifferentCoefficientsNotEqual)
	{
		CHECK(make({1, 5, 3}) != make({1, 2, 3}));
	}

	TEST(DifferentDegreesNotEqual)
	{
		CHECK(make({1, 2}) != make({1, 2, 3}));
	}

	TEST(DifferenceOnlyInLeadingCoefficientDetected)
	{
		CHECK(!(make({1, 2, 3}) == make({1, 2, 4})));
	}

	// ---------- Присваивание ----------
	TEST(AssignmentCopiesValue)
	{
		Polynomial a = make({1}), b = make({1, 2, 3});
		a = b;
		CHECK(a == b);
	}

	TEST(AssignmentWhenOnlyLeadingCoefficientDiffers)
	{
		Polynomial a = make({1, 2, 3}), b = make({1, 2, 4});
		a = b;
		CHECK_CLOSE(4.0, a[2], EPS);
	}

	TEST(SelfAssignmentKeepsValue)
	{
		Polynomial a = make({1, 2, 3});
		Polynomial &ref = a;
		a = ref;
		CHECK(make({1, 2, 3}) == a);
	}

	TEST(AssignmentIsIndependentCopy)
	{
		Polynomial a = make({1, 2}), b = make({7, 8, 9});
		a = b;
		b[0] = 0;
		CHECK_CLOSE(7.0, a[0], EPS);
	}

	// ---------- Вывод ----------
	TEST(OutputFullPolynomial)
	{
		CHECK_STR("3x^2 + 2x + 1", str(make({1, 2, 3})));
	}

	TEST(OutputOmitsUnitCoefficient)
	{
		CHECK_STR("x + 1", str(make({1, 1})));
	}

	TEST(OutputSkipsZeroTerms)
	{
		CHECK_STR("x^3 + 1", str(make({1, 0, 0, 1})));
	}

	TEST(OutputNegativeCoefficients)
	{
		CHECK_STR("3x^2 - 2x + 1", str(make({1, -2, 3})));
		CHECK_STR("-x^2 + 1", str(make({1, 0, -1})));
	}

	TEST(OutputFractionalCoefficients)
	{
		CHECK_STR("1.5x + 0.5", str(make({0.5, 1.5})));
	}

	// ---------- Ввод ----------
	TEST(InputReadsDegreeAndCoefficients)
	{
		Polynomial p(0);
		std::istringstream is("2 1 2 3");
		is >> p;
		CHECK_STR("3x^2 + 2x + 1", str(p));
	}

	TEST(InputNormalizesLeadingZeros)
	{
		Polynomial p(0);
		std::istringstream is("3 1 2 0 0");
		is >> p;
		CHECK_STR("2x + 1", str(p));
	}

	TEST(InputNegativeDegreeLeavesPolynomialUnchanged)
	{
		Polynomial p = make({1, 1});
		std::istringstream is("-1");
		is >> p;
		CHECK(make({1, 1}) == p);
	}

	TEST(InputInvalidTextSetsFailState)
	{
		Polynomial p(0);
		std::istringstream is("abc");
		CHECK(!(is >> p));
	}

	// ---------- Комбинированные ----------
	TEST(DistributiveLaw)
	{
		Polynomial a = make({1, 2}), b = make({3, 0, 1}), c = make({-1, 4});
		CHECK((a + b) * c == a * c + b * c);
	}
}
