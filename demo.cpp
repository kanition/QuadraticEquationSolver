#include <cmath>
#include <iomanip>
#include "QuadraticEquationSolver.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

void naive_solver(const double a, const double b, const double c, SolverState &t, double &x1, double &x2)
{
    constexpr double inf = std::numeric_limits<double>::infinity();
    const double nan = std::nan("");
    if (std::isnan(a) || std::isnan(b) || std::isnan(c) || std::isinf(a) || std::isinf(b) || std::isinf(c))
    {
        x1 = nan;
        x2 = nan;
        t = INVALID_INPUT;
        return;
    }
    if (a == 0)
    {
        if (b == 0)
        {
            if (c == 0)
            {
                x1 = inf;
                x2 = -inf;
                t = ALL_REAL;
                return;
            }
            else
            {
                x1 = nan;
                x2 = nan;
                t = NO_ROOT;
                return;
            }
        }
        else
        {
            x1 = -c / b;
            x2 = nan;
            t = ONE_REAL;
            return;
        }
    }
    double delta = (b * b - 4 * a * c);
    if (delta < 0)
    {
        x1 = nan;
        x2 = nan;
        t = NO_ROOT;
        return;
    }
    if (delta == 0)
    {
        x1 = -b / (2 * a);
        x2 = nan;
        t = ONE_REAL;
        return;
    }
    x1 = (-b - std::sqrt(delta)) / (2 * a);
    x2 = (-b + std::sqrt(delta)) / (2 * a);
    t = TWO_REAL;
}

static void print_iterm(const double m, const char *const iterm, bool &have_last)
{
    if (m == 0)
    {
        return;
    }
    if (have_last)
    {
        std::cout << " ";
    }
    if (m < 0)
    {
        std::cout << m << iterm;
        have_last = true;
        return;
    }
    if (have_last)
    {
        std::cout << "+";
    }
    std::cout << m << iterm;
    have_last = true;
}

void print_info(const double a, const double b, const double c)
{
    bool have_last = false;
    std::cout << "Solving: ";
    print_iterm(a, "x^2", have_last);
    print_iterm(b, "x", have_last);
    print_iterm(c, "", have_last);
    if (!have_last)
    {
        std::cout << "0";
    }
    std::cout << " = 0" << std::endl;
}

bool is_same_result(SolverState &s, double &r1, double &r2, SolverState &t, double &x1, double &x2)
{
    if (s != t)
    {
        return false;
    }
    switch (s)
    {
    case UNCERTAIN:
    case INVALID_INPUT:
    case ALL_REAL:
    case NO_ROOT:
    case OVER_UNDER_FLOW:
        return true;
    case ONE_REAL:
        return r1 == x1;
    case TWO_REAL:
        return (r1 == x1) && (r2 == x2);
    }
    return false;
}

void demo(const char *tips, const double a, const double b, const double c)
{
    static unsigned int i = 1;
    std::cout << "Case " << i++ << ": " << tips << std::endl;
    print_info(a, b, c);

    double r1, r2, x1, x2;
    SolverState s, t;

    QuadtraticEquationSolver solver(a, b, c);
    solver.solve(s, r1, r2);
    std::cout << std::endl
              << "QuadtraticEquationSolver results:" << std::endl;
    std::cout << "State = " << print_solver_state(s) << std::endl;
    std::cout << "r1 = " << r1 << std::endl;
    std::cout << "r2 = " << r2 << std::endl;

    naive_solver(a, b, c, t, x1, x2);
    std::cout << std::endl
              << "Naive results:" << std::endl;
    std::cout << "State = " << print_solver_state(t) << std::endl;
    std::cout << "x1 = " << x1 << std::endl;
    std::cout << "x2 = " << x2 << std::endl
              << std::endl;

    if (is_same_result(s, r1, r2, t, x1, x2))
    {
        std::cout << GREEN << "Same results" << RESET << std::endl;
    }
    else
    {
        std::cout << RED << "Different results" << RESET << std::endl;
    }
    std::cout << "=======================================" << std::endl
              << std::endl;
}

int main()
{
    constexpr double p = std::numeric_limits<double>::min();
    constexpr double q = std::numeric_limits<double>::max();
    std::cout << std::setprecision(15);
    std::cout << "double min: " << p << std::endl;
    std::cout << "double max: " << q << std::endl;
    std::cout << "=======================================" << std::endl
              << std::endl;

    demo("a = b = c = 0, all real numbers are the roots:", 0, 0, 0);

    demo("a = b = 0, but c!=0, no real root:", 0, 0, 4);
    demo("a = b = 0, c is small non-zero, no real root:", 0, 0, p);
    demo("a = b = 0, c is big non-zero, no real root:", 0, 0, q);

    demo("bx = 0, one real root", 0, 2, 0);
    demo("bx = 0, b is small non-zero, one real root", 0, p, 0);
    demo("bx = 0, b is big non-zero, one real root", 0, q, 0);

    demo("bx + c = 0, one real root", 0, 2, 8);
    demo("bx + c = 0, one big real root", 0, 2e-307, 8);
    demo("bx + c = 0, same root", 0, 2, 8e307);
    demo("bx + c = 0, one small real root", 0, 2e307, 8e-2);
    demo("bx + c = 0, same root", 0, 2, 8e-309);
    demo("bx + c = 0, overflow", 0, p, q);
    demo("bx + c = 0, underflow", 0, q, p);

    demo("ax^2 = 0", 5, 0, 0);
    demo("ax^2 + c = 0, but no real root", 5, 0, 7);
    demo("ax^2 + c = 0, small a and c, but no real root", p, 0, p);
    demo("ax^2 + c = 0, big a and c, but no real root", q, 0, q);
    demo("ax^2 + c = 0, big a and small c, but no real root", q, 0, p);
    demo("ax^2 + c = 0, small a and big c, but no real root", p, 0, q);

    demo("ax^2 + c = 0, two real roots", 1, 0, -9);
    demo("ax^2 + c = 0, same roots, but overflow", 1e307, 0, -9e307);
    demo("ax^2 + c = 0, same roots, but underflow", 1e-309, 0, -9e-309);
    demo("ax^2 + c = 0, two real roots", 1e-309, 0, -2.89e307);
    demo("ax^2 + c = 0, two roots, but underflow", 1e308, 0, -2.89e-308);

    demo("ax^2 + bx = 0, zero and non-zero roots", 4, 8, 0);
    demo("ax^2 + bx = 0, zero and non-zero roots", 4, -8, 0);
    demo("ax^2 + bx = 0, same roots, but overflow", 4e307, -8e307, 0);
    demo("ax^2 + bx = 0, same roots, but underflow", 4e-308, -8e-308, 0);

    demo("ax^2 + bx + c = 0, no real root", 2, 8, 10);
    demo("ax^2 + bx + c = 0, no real root, but underflow", 2e-308, 8e-308, 1e-307);
    demo("ax^2 + bx + c = 0, no real root, but overflow", 2e307, 8e307, 1e308);

    demo("ax^2 + bx + c = 0, one real root", 2, -8, 8);
    demo("ax^2 + bx + c = 0, same root, but overflow", 2e307, -8e307, 8e307);
    demo("ax^2 + bx + c = 0, same root", 3e-308, -1.2e-307, 1.2e-307);

    demo("ax^2 + bx + c = 0, two real roots", 6, -33, 45);
    demo("ax^2 + bx + c = 0, same roots, but underflow", 6e-308, -3.3e-307, 4.5e-307);
    demo("ax^2 + bx + c = 0, same roots, but overflow", 6e306, -3.3e307, 4.5e307);

    demo("Golden Ratio, two real roots, different from the 16th number, golden value is 0.618033988749894 84820458683436564", 1, 1, -1);
    return 0;
}