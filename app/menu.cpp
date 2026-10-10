#define NOMINMAX
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Polynomial.h"
#include "Set.h"

namespace
{
	void flushLine()
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	int readInt(const std::string &prompt)
	{
		while (true)
		{
			std::cout << prompt;
			int v;

			if (std::cin >> v)
			{
				flushLine();
				return v;
			}

			if (std::cin.eof())
				std::exit(0);

			flushLine();
			std::cout << "Ошибка ввода, попробуйте ещё раз.\n";
		}
	}

	double readDouble(const std::string &prompt)
	{
		while (true)
		{
			std::cout << prompt;
			double v;

			if (std::cin >> v)
			{
				flushLine();
				return v;
			}

			if (std::cin.eof())
				std::exit(0);

			flushLine();
			std::cout << "Ошибка ввода, попробуйте ещё раз.\n";
		}
	}

	std::string readLine(const std::string &prompt)
	{
		std::cout << prompt;
		std::string line;

		if (!std::getline(std::cin, line))
			std::exit(0);

		return line;
	}

	void readPolynomial(const char *name, Polynomial &p)
	{
		std::cout << "Введите " << name << ": степень n, затем n+1 коэффициентов\n"
		          << "(от свободного члена к старшему), через пробел.\nПример: 2 1 2 3  =  3x^2 + 2x + 1\n> ";

		if (!(std::cin >> p))
		{
			if (std::cin.eof())
				std::exit(0);

			std::cout << "Ошибка ввода, многочлен не изменён.\n";
		}

		flushLine();
	}

	void polynomialMenu()
	{
		Polynomial a(0), b(0);
		int choice;

		do
		{
			std::cout << "\n===== Polynomial =====\n"
			          << "A = " << a << "\n"
			          << "B = " << b << "\n"
			          << " 1. Ввести A            2. Ввести B\n"
			          << " 3. A + B               4. A - B\n"
			          << " 5. A * B               6. A / B\n"
			          << " 7. Значение A(x)       8. Сравнить A и B (==, !=)\n"
			          << " 9. A += B             10. A -= B\n"
			          << "11. A *= B             12. A /= B\n"
			          << "13. A = B              14. Прочитать A[i]\n"
			          << "15. Изменить A[i]\n"
			          << " 0. Назад\n";

			choice = readInt("Выбор: ");

			try
			{
				switch (choice)
				{
				case 1: readPolynomial("A", a); break;
				case 2: readPolynomial("B", b); break;
				case 3: std::cout << "A + B = " << (a + b) << "\n"; break;
				case 4: std::cout << "A - B = " << (a - b) << "\n"; break;
				case 5: std::cout << "A * B = " << (a * b) << "\n"; break;
				case 6: std::cout << "A / B = " << (a / b) << "\n"; break;
				case 7:
				{
					double x = readDouble("x = ");
					std::cout << "A(" << x << ") = " << a(x) << "\n";
					break;
				}
				case 8:
					std::cout << "A == B: " << (a == b ? "да" : "нет") << "\n"
					          << "A != B: " << (a != b ? "да" : "нет") << "\n";
					break;
				case 9: a += b; std::cout << "A = " << a << "\n"; break;
				case 10: a -= b; std::cout << "A = " << a << "\n"; break;
				case 11: a *= b; std::cout << "A = " << a << "\n"; break;
				case 12: a /= b; std::cout << "A = " << a << "\n"; break;
				case 13: a = b; std::cout << "A = " << a << "\n"; break;
				case 14:
				{
					int i = readInt("Индекс i = ");
					std::cout << "A[" << i << "] = " << a[i] << "\n";
					break;
				}
				case 15:
				{
					int i = readInt("Индекс i = ");
					a[i] = readDouble("Новое значение = ");
					std::cout << "A = " << a << "\n";
					break;
				}
				case 0: break;
				default: std::cout << "Нет такого пункта.\n";
				}
			}
			catch (const std::exception &e)
			{
				std::cout << "Исключение: " << e.what() << "\n";
			}
		} while (choice != 0);
	}

	void setMenu()
	{
		Set a, b;
		int choice;

		do
		{
			std::cout << "\n===== Set =====\n"
			          << "A = " << a << "   (размер " << a.size() << ")\n"
			          << "B = " << b << "   (размер " << b.size() << ")\n"
			          << " 1. Ввести A (строкой)   2. Ввести B (строкой)\n"
			          << " 3. Добавить атом в A    4. Добавить подмножество в A\n"
			          << " 5. Размер A             6. A пусто?\n"
			          << " 7. Сравнить A и B (==, !=)\n"
			          << " 8. A = B                9. Копия A (конструктор копирования)\n"
			          << " 0. Назад\n"
			          << "Пример ввода множества: {a, b, {c, d}, {}}\n";

			choice = readInt("Выбор: ");

			try
			{
				switch (choice)
				{
				case 1: std::cout << "A = "; std::cin >> a; break;
				case 2: std::cout << "B = "; std::cin >> b; break;
				case 3: a.add(SetElement(readLine("Атом: "))); break;
				case 4: a.add(SetElement(Set(readLine("Подмножество, например {x, y}: ")))); break;
				case 5: std::cout << "|A| = " << a.size() << "\n"; break;
				case 6: std::cout << (a.empty() ? "A пусто" : "A не пусто") << "\n"; break;
				case 7:
					std::cout << "A == B: " << (a == b ? "да" : "нет") << "\n"
					          << "A != B: " << (a != b ? "да" : "нет") << "\n";
					break;
				case 8: a = b; std::cout << "A = " << a << "\n"; break;
				case 9:
				{
					Set copy(a);
					std::cout << "Копия: " << copy << "\n"
					          << "Копия == A: " << (copy == a ? "да" : "нет") << "\n";
					break;
				}
				case 0: break;
				default: std::cout << "Нет такого пункта.\n";
				}
			}
			catch (const std::exception &e)
			{
				std::cout << "Исключение: " << e.what() << "\n";
			}
		} while (choice != 0);
	}
}

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	int choice;

	do
	{
		std::cout << "\n===== ГЛАВНОЕ МЕНЮ =====\n"
		          << "1. Тестировать Polynomial\n"
		          << "2. Тестировать Set\n"
		          << "0. Выход\n";

		choice = readInt("Выбор: ");

		if (choice == 1) polynomialMenu();
		else if (choice == 2) setMenu();
		else if (choice != 0) std::cout << "Нет такого пункта.\n";
	} while (choice != 0);

	return 0;
}
