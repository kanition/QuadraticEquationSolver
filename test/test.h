#pragma once
#include <iomanip>
#include <sstream>
#include <iostream>
#include "QuadraticEquationSolver.h"

constexpr auto RESET = "\033[0m";
constexpr auto RED = "\033[31m";
constexpr auto GREEN = "\033[32m";

template <typename T>
void naive_solver(const T a, const T b, const T c, SolverState &s, T &x1, T &x2)
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
        s = INVALID_INPUT;
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
                s = ALL_REAL;
                goto CHECK_OVERUNDERFLOW;
            }
            else
            {
                x1 = nan;
                x2 = nan;
                s = NO_ROOT;
                goto CHECK_OVERUNDERFLOW;
            }
        }
        else
        {
            x1 = -c / b;
            x2 = nan;
            s = ONE_REAL;
            goto CHECK_OVERUNDERFLOW;
        }
    }
    if (delta < 0)
    {
        x1 = nan;
        x2 = nan;
        s = NO_ROOT;
        goto CHECK_OVERUNDERFLOW;
    }
    if (delta == 0)
    {
        x1 = -b / (two * a);
        x2 = nan;
        s = ONE_REAL;
        goto CHECK_OVERUNDERFLOW;
    }
    x1 = (-b - std::sqrt(delta)) / (two * a);
    x2 = (-b + std::sqrt(delta)) / (two * a);
    s = TWO_REAL;
CHECK_OVERUNDERFLOW:
#define is_invalid_root(x) ((std::isnan((x))) || (std::isinf((x))))
    if (((TWO_REAL == s) && (is_invalid_root(x1) || is_invalid_root(x2))) || (ONE_REAL == s && is_invalid_root(x1)))
    {
        s = OVER_UNDER_FLOW;
    }
#undef is_invalid_root
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
        std::cout << "- " << -m << iterm;
        have_last = true;
        return;
    }
    if (have_last)
    {
        std::cout << "+ ";
    }
    std::cout << m << iterm;
    have_last = true;
}

template <typename T>
void print_info(const T a, const T b, const T c)
{
    bool have_last = false;
    std::cout << "Solving: ";
    print_iterm(a, " * x^2", have_last);
    print_iterm(b, " * x", have_last);
    print_iterm(c, " ", have_last);
    if (!have_last)
    {
        std::cout << "0";
    }
    std::cout << "= 0" << std::endl;
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

static std::string str_center(const std::string &tips, const size_t w, const size_t p)
{
    const size_t i = (w - tips.size()) / 2;
    std::string info = std::string(i, ' ') + tips + std::string(w - i - tips.size(), ' ');
    return info;
}

template <typename T>
std::string num_center(const T x, const size_t w, const size_t p)
{
    std::stringstream stream;
    stream << std::setprecision(p) << x;
    return str_center(stream.str(), w, p);
}

template <typename T>
void demo(const char *tips, const T a, const T b, const T c)
{
    T r1(0), r2(0), x1(0), x2(0);
    SolverState s(UNCERTAIN), t(UNCERTAIN);
    QuadtraticEquationSolver<T> solver(a, b, c);
    s = solver.solve(r1, r2);
    naive_solver<T>(a, b, c, t, x1, x2);

    const bool is_same = is_same_result(s, r1, r2, t, x1, x2);
    const auto color = is_same ? GREEN : RED;
    constexpr size_t precs = std::is_same_v<T, double> ? 15 : 7;
    const std::string data_type = std::is_same_v<T, double> ? "double" : "float";
    static size_t ind = 1;
    std::cout << "(" << ind++ << ") " << tips << std::endl;
    print_info(a, b, c);
    const std::string info1 = QuadtraticEquationSolver<T>::print_solver_state(s);
    const std::string info2 = QuadtraticEquationSolver<T>::print_solver_state(t);
    constexpr size_t w = 28;
    std::cout << "+-------+" << std::string(w, '-') << "+" << std::string(w, '-') << "+" << std::endl;
    std::cout << "|" << str_center(data_type, 7, precs) << "|  QuadtraticEquationSolver  "
              << "|        Naive Solver        |" << std::endl;
    std::cout << "+-------+" << std::string(w, '-') << "+" << std::string(w, '-') << "+" << std::endl;
    std::cout << "| state |" << str_center(info1, w, precs) << "|" << str_center(info2, w, precs) << "|" << std::endl;
    std::cout << "|  x_1  |" << num_center(r1, w, precs) << "|" << num_center(x1, w, precs) << "|" << std::endl;
    std::cout << "|  x_2  |" << num_center(r2, w, precs) << "|" << num_center(x2, w, precs) << "|" << std::endl;
    std::cout << "|  gap  |" << color << str_center(is_same ? "Same" : "Different", 2 * w + 1, precs) << RESET << "|" << std::endl;
    std::cout << "+-------+" << std::string(w, '-') << "+" << std::string(w, '-') << "+\n"
              << std::endl;
}
