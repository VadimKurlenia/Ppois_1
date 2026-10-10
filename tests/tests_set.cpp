#include "ut.h"
#include "Set.h"
#include <sstream>

namespace
{
	std::string str(const Set &s)
	{
		std::ostringstream os;
		os << s;
		return os.str();
	}

	SetElement atom(const std::string &s) { return SetElement(s); }
	SetElement sub(const std::string &s) { return SetElement(Set(s)); }
}

SUITE(SetTests)
{
	// ---------- Создание и разбор строки ----------
	TEST(DefaultSetIsEmpty)
	{
		Set s;
		CHECK(s.empty());
		CHECK_EQUAL(0u, s.size());
		CHECK_STR("{}", str(s));
	}

	TEST(EmptyBracesParseToEmptySet)
	{
		CHECK(Set("{}").empty());
	}

	TEST(EmptyStringParsesToEmptySet)
	{
		CHECK(Set("").empty());
	}

	TEST(ParseAtoms)
	{
		Set s("{a, b, c}");
		CHECK_EQUAL(3u, s.size());
	}

	TEST(ParseWithoutOuterBraces)
	{
		Set s("a, b");
		CHECK_EQUAL(2u, s.size());
		CHECK_STR("{a, b}", str(s));
	}

	TEST(ParseTrimsSpaces)
	{
		CHECK_STR("{a, b}", str(Set("{   a ,    b   }")));
	}

	TEST(ParseTrimsTabsAndNewlines)
	{
		CHECK_STR("{a, b}", str(Set("{\ta,\n b}")));
	}

	TEST(ParseRemovesDuplicateAtoms)
	{
		Set s("{a, b, a, b, a}");
		CHECK_EQUAL(2u, s.size());
	}

	TEST(ParseSortsAtoms)
	{
		CHECK_STR("{a, b, c}", str(Set("{c, a, b}")));
	}

	TEST(ParseIgnoresEmptyTokens)
	{
		CHECK_EQUAL(2u, Set("{a,,b}").size());
		CHECK_EQUAL(2u, Set("{a, b,}").size());
	}

	TEST(NumericAtomsSortedAsStrings)
	{
		CHECK_STR("{1, 10, 2}", str(Set("{2, 10, 1}")));
	}

	TEST(UppercaseGoesBeforeLowercase)
	{
		CHECK_STR("{B, a}", str(Set("{a, B}")));
	}

	TEST(ConstructFromStdString)
	{
		std::string text = "{x, y}";
		Set s(text);
		CHECK_EQUAL(2u, s.size());
	}

	// ---------- Вложенные множества ----------
	TEST(ParseNestedSet)
	{
		Set s("{a, {b, c}}");
		CHECK_EQUAL(2u, s.size());
		CHECK_STR("{a, {b, c}}", str(s));
	}

	TEST(ParseSetContainingEmptySet)
	{
		Set s("{{}}");
		CHECK_EQUAL(1u, s.size());
		CHECK_STR("{{}}", str(s));
	}

	TEST(ParseDeepNesting)
	{
		CHECK_STR("{{{a}}}", str(Set("{{{a}}}")));
	}

	TEST(AtomsPrintedBeforeSubsets)
	{
		CHECK_STR("{b, {a}}", str(Set("{{a}, b}")));
	}

	TEST(AtomAndEmptySetCoexist)
	{
		Set s("{a, {}}");
		CHECK_EQUAL(2u, s.size());
		CHECK_STR("{a, {}}", str(s));
	}

	TEST(DuplicateSubsetsCollapse)
	{
		CHECK_EQUAL(1u, Set("{{a, b}, {b, a}}").size());
	}

	TEST(NestedSetsEqualRegardlessOfOrder)
	{
		CHECK(Set("{{a, b}, {c}}") == Set("{{c}, {b, a}}"));
	}

	// ---------- add / size / empty ----------
	TEST(AddAtomToEmptySet)
	{
		Set s;
		s.add(atom("x"));
		CHECK(!s.empty());
		CHECK_EQUAL(1u, s.size());
	}

	TEST(AddDuplicateAtomIsIgnored)
	{
		Set s("{x}");
		s.add(atom("x"));
		CHECK_EQUAL(1u, s.size());
	}

	TEST(AddKeepsElementsSorted)
	{
		Set s;
		s.add(atom("b"));
		s.add(atom("a"));
		CHECK_STR("{a, b}", str(s));
	}

	TEST(AddSubset)
	{
		Set s("{a}");
		s.add(sub("{b}"));
		CHECK_EQUAL(2u, s.size());
		CHECK_STR("{a, {b}}", str(s));
	}

	TEST(AddDuplicateSubsetIsIgnored)
	{
		Set s("{{a, b}}");
		s.add(sub("{b, a}"));
		CHECK_EQUAL(1u, s.size());
	}

	TEST(AddEmptySetAsElement)
	{
		Set s;
		s.add(sub("{}"));
		CHECK_EQUAL(1u, s.size());
		CHECK_STR("{{}}", str(s));
	}

	// ---------- Сравнение множеств ----------
	TEST(EmptySetsAreEqual)
	{
		CHECK(Set() == Set("{}"));
	}

	TEST(SetsEqualRegardlessOfInputOrder)
	{
		CHECK(Set("{a, b, c}") == Set("{c, b, a}"));
	}

	TEST(DifferentSizesNotEqual)
	{
		CHECK(Set("{a}") != Set("{a, b}"));
	}

	TEST(DifferentElementsNotEqual)
	{
		CHECK(Set("{a, b}") != Set("{a, c}"));
	}

	TEST(AtomAndSingletonSetDiffer)
	{
		CHECK(Set("{a}") != Set("{{a}}"));
	}

	TEST(NotEqualIsNegationOfEqual)
	{
		Set a("{a, b}"), b("{a, b}");
		CHECK(a == b);
		CHECK(!(a != b));
	}

	// ---------- Копирование и присваивание ----------
	TEST(CopyConstructorCopiesElements)
	{
		Set a("{a, {b}}");
		Set b(a);
		CHECK(a == b);
	}

	TEST(CopyIsIndependentFromOriginal)
	{
		Set a("{a}");
		Set b(a);
		b.add(atom("z"));
		CHECK_EQUAL(1u, a.size());
		CHECK_EQUAL(2u, b.size());
	}

	TEST(AssignmentOverwritesContents)
	{
		Set a("{a}"), b("{x, y, z}");
		a = b;
		CHECK(a == b);
		CHECK_EQUAL(3u, a.size());
	}

	TEST(SelfAssignmentKeepsValue)
	{
		Set a("{a, b}");
		Set &ref = a;
		a = ref;
		CHECK_STR("{a, b}", str(a));
	}

	// ---------- SetElement ----------
	TEST(ElementsWithSameAtomAreEqual)
	{
		CHECK(atom("a") == atom("a"));
	}

	TEST(ElementsWithDifferentAtomsDiffer)
	{
		CHECK(atom("a") != atom("b"));
	}

	TEST(AtomAndSubsetElementsDiffer)
	{
		CHECK(atom("a") != sub("{a}"));
	}

	TEST(SubsetElementsCompareByContent)
	{
		CHECK(sub("{a, b}") == sub("{b, a}"));
		CHECK(sub("{a}") != sub("{b}"));
	}

	TEST(ElementOrderingAtoms)
	{
		CHECK(atom("a") < atom("b"));
		CHECK(!(atom("b") < atom("a")));
	}

	TEST(AtomIsLessThanSubset)
	{
		CHECK(atom("z") < sub("{a}"));
		CHECK(!(sub("{a}") < atom("z")));
	}

	TEST(ElementOrderingSubsets)
	{
		CHECK(sub("{a}") < sub("{b}"));
	}

	// ---------- Ввод / вывод ----------
	TEST(OutputOfEmptySet)
	{
		CHECK_STR("{}", str(Set()));
	}

	TEST(OutputOfFlatSet)
	{
		CHECK_STR("{a, b, c}", str(Set("{a, b, c}")));
	}

	TEST(InputReadsOneLine)
	{
		Set s;
		std::istringstream is("{a, b, c}\n");
		is >> s;
		CHECK_EQUAL(3u, s.size());
	}

	TEST(InputReadsNestedSet)
	{
		Set s;
		std::istringstream is("{a, {b, {c}}}\n");
		is >> s;
		CHECK_STR("{a, {b, {c}}}", str(s));
	}

	TEST(InputEmptyLineGivesEmptySet)
	{
		Set s("{a}");
		std::istringstream is("\n");
		is >> s;
		CHECK(s.empty());
	}

	TEST(OutputThenParseRoundTrip)
	{
		Set s("{c, {b, a}, {}, d}");
		CHECK(s == Set(str(s)));
	}
}
