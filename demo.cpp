#include <iomanip>
#include <iostream>
#include "QuadraticEquationSolver.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

#define is_invalid_root(x) ((std::isnan((x))) || (std::isinf((x))))

template <typename T>
void naive_solver(const T a, const T b, const T c, SolverState &t, T &x1, T &x2)
{
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, float>,
                  "Only support float or double type");
    constexpr T two = 2;
    constexpr T four = 4;
    constexpr T inf = std::numeric_limits<T>::infinity();
    T temp;
    if (std::is_same_v<T, double>)
    {
        temp = static_cast<T>(std::nan(""));
    }
    else
    {
        temp = static_cast<T>(std::nanf(""));
    }
    const T nan = temp;
    T delta = (b * b - four * a * c);
    if (std::isnan(a) || std::isnan(b) || std::isnan(c) || std::isinf(a) || std::isinf(b) || std::isinf(c))
    {
        x1 = nan;
        x2 = nan;
        t = INVALID_INPUT;
        goto CHECK_OVERUNDERFLOW;
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
                goto CHECK_OVERUNDERFLOW;
            }
            else
            {
                x1 = nan;
                x2 = nan;
                t = NO_ROOT;
                goto CHECK_OVERUNDERFLOW;
            }
        }
        else
        {
            x1 = -c / b;
            x2 = nan;
            t = ONE_REAL;
            goto CHECK_OVERUNDERFLOW;
        }
    }
    if (delta < 0)
    {
        x1 = nan;
        x2 = nan;
        t = NO_ROOT;
        goto CHECK_OVERUNDERFLOW;
    }
    if (delta == 0)
    {
        x1 = -b / (two * a);
        x2 = nan;
        t = ONE_REAL;
        goto CHECK_OVERUNDERFLOW;
    }
    x1 = (-b - std::sqrt(delta)) / (two * a);
    x2 = (-b + std::sqrt(delta)) / (two * a);
    t = TWO_REAL;
CHECK_OVERUNDERFLOW:
    if (((TWO_REAL == t) && (is_invalid_root(x1) || is_invalid_root(x2))) || (ONE_REAL == t && is_invalid_root(x1)))
    {
        t = OVER_UNDER_FLOW;
    }
}

template <typename T>
static void print_iterm(const T m, const char *const iterm, bool &have_last)
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

template <typename T>
void print_info(const T a, const T b, const T c)
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

template <typename T>
bool is_same_result(SolverState &s, T &r1, T &r2, SolverState &t, T &x1, T &x2)
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

template <typename T>
void demo(const char *tips, const T a, const T b, const T c)
{
    static unsigned int i = 1;
    constexpr T p = std::numeric_limits<T>::min();
    constexpr T q = std::numeric_limits<T>::max();
    constexpr int precision = std::is_same_v<T, double> ? 15 : 7;
    const std::string data_type = std::is_same_v<T, double> ? "double" : "float";
    if (i == 1)
    {
        std::cout << std::setprecision(precision);
        std::cout << data_type << " min: " << p << std::endl;
        std::cout << data_type << " max: " << q << std::endl;
        std::cout << "=======================================" << std::endl
                  << std::endl;
    }
    std::cout << "Case " << i++ << ": " << tips << std::endl;
    print_info(a, b, c);

    T r1, r2, x1, x2;
    SolverState s, t;

    QuadtraticEquationSolver<T> solver(a, b, c);
    solver.solve(s, r1, r2);
    std::cout << std::endl
              << "QuadtraticEquationSolver results:" << std::endl;
    std::cout << "State = " << print_solver_state(s) << std::endl;
    std::cout << "r1 = " << r1 << std::endl;
    std::cout << "r2 = " << r2 << std::endl;

    naive_solver<T>(a, b, c, t, x1, x2);
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

void test_double()
{
    constexpr double p = std::numeric_limits<double>::min();
    constexpr double q = std::numeric_limits<double>::max();
    demo("a = b = c = 0, all real numbers are the roots:", 0., 0., 0.);

    demo("a = b = 0, but c!=0, no real root:", 0., 0., 4.);
    demo("a = b = 0, c is small non-zero, no real root:", 0., 0., p);
    demo("a = b = 0, c is big non-zero, no real root:", 0., 0., q);

    demo("bx = 0, one real root", 0., 2., 0.);
    demo("bx = 0, b is small non-zero, one real root", 0., p, 0.);
    demo("bx = 0, b is big non-zero, one real root", 0., q, 0.);

    demo("bx + c = 0, one real root", 0., 2., 8.);
    demo("bx + c = 0, one big real root", 0., 2e-307, 8.);
    demo("bx + c = 0, same root", 0., 2., 8e307);
    demo("bx + c = 0, one small real root", 0., 2e307, 8e-2);
    demo("bx + c = 0, same root", 0., 2., 8e-309);
    demo("bx + c = 0, overflow", 0., p, q);
    demo("bx + c = 0, underflow", 0., q, p);

    demo("ax^2 = 0", 5., 0., 0.);
    demo("ax^2 + c = 0, but no real root", 5., 0., 7.);
    demo("ax^2 + c = 0, small a and c, but no real root", p, 0., p);
    demo("ax^2 + c = 0, big a and c, but no real root", q, 0., q);
    demo("ax^2 + c = 0, big a and small c, but no real root", q, 0., p);
    demo("ax^2 + c = 0, small a and big c, but no real root", p, 0., q);

    demo("ax^2 + c = 0, two real roots", 1., 0., -9.);
    demo("ax^2 + c = 0, same roots, but overflow", 1e307, 0., -9e307);
    demo("ax^2 + c = 0, same roots, but underflow", 1e-309, 0., -9e-309);
    demo("ax^2 + c = 0, two real roots", 1e-309, 0., -2.89e307);
    demo("ax^2 + c = 0, two roots, but underflow", 1e308, 0., -2.89e-308);

    demo("ax^2 + bx = 0, zero and non-zero roots", 4., 8., 0.);
    demo("ax^2 + bx = 0, zero and non-zero roots", 4., -8., 0.);
    demo("ax^2 + bx = 0, same roots, but overflow", 4e307, -8e307, 0.);
    demo("ax^2 + bx = 0, same roots, but underflow", 4e-308, -8e-308, 0.);

    demo("ax^2 + bx + c = 0, no real root", 2., 8., 10.);
    demo("ax^2 + bx + c = 0, no real root, but underflow", 2e-308, 8e-308, 1e-307);
    demo("ax^2 + bx + c = 0, no real root, but overflow", 2e307, 8e307, 1e308);

    demo("ax^2 + bx + c = 0, one real root", 2., -8., 8.);
    demo("ax^2 + bx + c = 0, same root, but overflow", 2e307, -8e307, 8e307);
    demo("ax^2 + bx + c = 0, same root", 3e-308, -1.2e-307, 1.2e-307);

    demo("ax^2 + bx + c = 0, two real roots", 6., -33., 45.);
    demo("ax^2 + bx + c = 0, same roots, but underflow", 6e-308, -3.3e-307, 4.5e-307);
    demo("ax^2 + bx + c = 0, same roots, but overflow", 6e306, -3.3e307, 4.5e307);

    demo("Golden Ratio, two real roots, different from the 15th number, golden value is 0.618033988749894(8)", 1., 1., -1.);
}

void test_float()
{
    constexpr float p = std::numeric_limits<float>::min();
    constexpr float q = std::numeric_limits<float>::max();
    demo("a = b = c = 0, all real numbers are the roots:", 0.f, 0.f, 0.f);

    demo("a = b = 0, but c!=0, no real root:", 0.f, 0.f, 4.f);
    demo("a = b = 0, c is small non-zero, no real root:", 0.f, 0.f, p);
    demo("a = b = 0, c is big non-zero, no real root:", 0.f, 0.f, q);

    demo("bx = 0, one real root", 0.f, 2.f, 0.f);
    demo("bx = 0, b is small non-zero, one real root", 0.f, p, 0.f);
    demo("bx = 0, b is big non-zero, one real root", 0.f, q, 0.f);

    demo("bx + c = 0, one real root", 0.f, 2.f, 8.f);
    demo("bx + c = 0, one big real root", 0.f, 2e-37f, 8.f);
    demo("bx + c = 0, same root", 0.f, 2.f, 8e37f);
    demo("bx + c = 0, one small real root", 0.f, 2e30f, 8e-7f);
    demo("bx + c = 0, same root", 0.f, 2.f, 8e-37f);
    demo("bx + c = 0, overflow", 0.f, p, q);
    demo("bx + c = 0, underflow", 0.f, q, p);

    demo("ax^2 = 0", 5.f, 0.f, 0.f);
    demo("ax^2 + c = 0, but no real root", 5.f, 0.f, 7.f);
    demo("ax^2 + c = 0, small a and c, but no real root", p, 0.f, p);
    demo("ax^2 + c = 0, big a and c, but no real root", q, 0.f, q);
    demo("ax^2 + c = 0, big a and small c, but no real root", q, 0.f, p);
    demo("ax^2 + c = 0, small a and big c, but no real root", p, 0.f, q);

    demo("ax^2 + c = 0, two real roots", 1.f, 0.f, -9.f);
    demo("ax^2 + c = 0, same roots, but overflow", 1e37f, 0.f, -9e37f);
    demo("ax^2 + c = 0, same roots, but underflow", 1e-37f, 0.f, -9e-37f);
    demo("ax^2 + c = 0, two real roots", 1e-37f, 0.f, -2.89e37f);
    demo("ax^2 + c = 0, two roots, but underflow", 1e37f, 0.f, -2.89e-37f);

    demo("ax^2 + bx = 0, zero and non-zero roots", 4.f, 8.f, 0.f);
    demo("ax^2 + bx = 0, zero and non-zero roots", 4.f, -8.f, 0.f);
    demo("ax^2 + bx = 0, same roots, but overflow", 4e37f, -8e37f, 0.f);
    demo("ax^2 + bx = 0, same roots, but underflow", 4e-37f, -8e-37f, 0.f);

    demo("ax^2 + bx + c = 0, no real root", 2.f, 8.f, 10.f);
    demo("ax^2 + bx + c = 0, no real root, but underflow", 2e-38f, 8e-38f, 1e-37f);
    demo("ax^2 + bx + c = 0, no real root, but overflow", 2e37f, 8e37f, 1e38f);

    demo("ax^2 + bx + c = 0, one real root", 2.f, -8.f, 8.f);
    demo("ax^2 + bx + c = 0, same root, but overflow", 2e37f, -8e37f, 8e37f);
    demo("ax^2 + bx + c = 0, same root", 3e-38f, -1.2e-37f, 1.2e-37f);

    demo("ax^2 + bx + c = 0, two real roots", 6.f, -33.f, 45.f);
    demo("ax^2 + bx + c = 0, same roots, but underflow", 6e-38f, -3.3e-37f, 4.5e-37f);
    demo("ax^2 + bx + c = 0, same roots, but overflow", 6e36f, -3.3e37f, 4.5e37f);

    demo("Golden Ratio, two real roots, different from the 7th number, golden value is 0.6180339(8)", 1.f, 1.f, -1.f);
}

int main()
{
    test_double();
    test_float();
    return 0;
}