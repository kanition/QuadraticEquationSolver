#pragma once

#ifndef _QUADRATIC_EQUATION_SOLVER_
#define _QUADRATIC_EQUATION_SOLVER_

#include <limits>
#include <cmath>
#include <type_traits>

#define sign(x) (((x) < 0) ? (-1) : (1))
#define is_invalid_input(x) ((std::isnan((x))) || (std::isinf((x))))

enum SolverState
{
    UNCERTAIN,
    INVALID_INPUT,
    ALL_REAL,
    NO_ROOT,
    ONE_REAL,
    TWO_REAL,
    OVER_UNDER_FLOW
};

template <typename T>
class QuadtraticEquationSolver
{
public:
    QuadtraticEquationSolver(const T a, const T b, const T c);
    ~QuadtraticEquationSolver();
    SolverState solve(T &r1, T &r2);
    void reset(const T a, const T b, const T c);
    static const std::string print_solver_state(SolverState s);
    const std::string print_solver_state();

private:
    T a;
    T b;
    T c;
    T x1;
    T x2;
    SolverState state;
    static T nan();
    void invalid_input();
    void solve();
    void all_real();
    void no_root();
    void one_real(const T x);
    static void low_high_sort(const T y1, const T y2, T &z1, T &z2);
    static void keep_exponent(const int m, int &m1, int &m2);
    static void veltkamp_split(const T x, T &xhigh, T &xlow);
    static T exactmult(const T x, const T y, const T pxy);
    static T kahan_discriminant(const T a, const T b, const T c);
    void solve_linear();
    void sqrt_minus_c_div_a();
    void solve_axx_plus_c();
    void solve_axx_plus_bx();
    void solve_complete();
    static constexpr T inf = std::numeric_limits<T>::infinity();
    static constexpr int n_bit_e = std::is_same_v<T, double> ? 11 : 8;
    static constexpr int n_bit_f = std::is_same_v<T, double> ? 52 : 23;
    static constexpr int m_max = (1 << (n_bit_e - 1)) - 1;
    static constexpr int m_min = 1 - m_max;
};

template <typename T>
QuadtraticEquationSolver<T>::QuadtraticEquationSolver(const T a, const T b, const T c)
    : a(a), b(b), c(c), x1(0), x2(0), state(UNCERTAIN)
{
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, float>, "Only support float or double type");
    static_assert(std::numeric_limits<T>::is_iec559, "Not Support IEC 559 / IEEE 754 standard");
}

template <typename T>
QuadtraticEquationSolver<T>::~QuadtraticEquationSolver()
{
}

template <typename T>
T QuadtraticEquationSolver<T>::nan()
{
    if (std::is_same_v<T, double>)
    {
        return static_cast<T>(std::nan(""));
    }
    else
    {
        return static_cast<T>(std::nanf(""));
    }
}

template <typename T>
void QuadtraticEquationSolver<T>::invalid_input()
{
    state = INVALID_INPUT;
}

template <typename T>
void QuadtraticEquationSolver<T>::all_real()
{
    state = ALL_REAL;
    x1 = inf;
    x2 = -inf;
}

template <typename T>
void QuadtraticEquationSolver<T>::no_root()
{
    state = NO_ROOT;
    x1 = nan();
    x2 = nan();
}

template <typename T>
void QuadtraticEquationSolver<T>::one_real(const T x)
{
    state = ONE_REAL;
    x1 = x;
    x2 = nan();
}

template <typename T>
void QuadtraticEquationSolver<T>::low_high_sort(const T y1, const T y2, T &z1, T &z2)
{
    if (y1 < y2)
    {
        z1 = y1;
        z2 = y2;
    }
    else
    {
        z1 = y2;
        z2 = y1;
    }
}

template <typename T>
void QuadtraticEquationSolver<T>::keep_exponent(const int m, int &m1, int &m2)
{
    if (m_min <= m && m <= m_max)
    {
        m1 = m;
        m2 = 0;
        return;
    }
    if (m < m_min)
    {
        m1 = m_min;
        m2 = m - m_min;
        return;
    }
    m1 = m_max;
    m2 = m - m_max;
}

template <typename T>
void QuadtraticEquationSolver<T>::veltkamp_split(const T x, T &xhigh, T &xlow)
{
    // split x = xhigh + xlow, and xhigh only uses high fraction bit, xlow only uses low fraction bits
    constexpr int coff = (1 << ((n_bit_f >> 1) + 1)) + 1;
    T gamma = coff * x;
    T delta = x - gamma;
    xhigh = gamma + delta;
    xlow = x - xhigh;
}

template <typename T>
T QuadtraticEquationSolver<T>::exactmult(const T x, const T y, const T pxy)
{
    T xhi(0), xlo(0), yhi(0), ylo(0);
    veltkamp_split(x, xhi, xlo);
    veltkamp_split(y, yhi, ylo);
    T t1 = -pxy + xhi * yhi;
    T t2 = t1 + xhi * ylo;
    T t3 = t2 + xlo * yhi;
    T e = t3 + xlo * ylo;
    return e;
}

template <typename T>
T QuadtraticEquationSolver<T>::kahan_discriminant(const T a, const T b, const T c)
{
    constexpr T th = 3;
    constexpr T four = 4;
    T p = b * b;
    T q = four * a * c;
    T d = p - q;
    if (th * std::fabs(d) >= (p + q))
    {
        // b*b and 4ac are different enough
        return d;
    }
    T dp = exactmult(b, b, p);
    T dq = exactmult(four * a, c, q);
    d = d + (dp - dq);
    return d;
}

template <typename T>
void QuadtraticEquationSolver<T>::solve_linear()
{
    if (b == 0)
    {
        if (c == 0)
        {
            all_real();
            return;
        }
        else
        {
            no_root();
            return;
        }
    }
    else
    {
        if (c == 0)
        {
            one_real(0);
            return;
        }
        else
        {
            one_real(-c / b);
            return;
        }
    }
}

template <typename T>
void QuadtraticEquationSolver<T>::sqrt_minus_c_div_a()
{
    constexpr T two = 2;
    int ea, ec;
    T a2, c2;
    a2 = frexp(a, &ea);
    c2 = frexp(c, &ec);
    int ecp = ec - ea;
    int m = (ecp & (~1)) >> 1; // even number towards zero, then divided by 2
    T c3 = c2 * std::pow(two, static_cast<T>(ecp & 1));
    T s = std::sqrt(-c3 / a2);
    int m1, m2;
    keep_exponent(m, m1, m2);
    x2 = (s * std::pow(two, static_cast<T>(m2))) * std::pow(two, static_cast<T>(m1));
    x1 = -x2;
    state = TWO_REAL;
}

template <typename T>
void QuadtraticEquationSolver<T>::solve_axx_plus_c()
{
    if (c == 0)
    {
        one_real(0); // two equal root
        return;
    }
    else
    {
        if (sign(a) == sign(c))
        {
            no_root(); // or complex root
            return;
        }
        else
        {
            sqrt_minus_c_div_a();
            return;
        }
    }
}

template <typename T>
void QuadtraticEquationSolver<T>::solve_axx_plus_bx()
{
    if (sign(a) == sign(b))
    {
        x1 = -b / a;
        x2 = 0;
        state = TWO_REAL;
        return;
    }
    else
    {
        x1 = 0;
        x2 = -b / a;
        state = TWO_REAL;
        return;
    }
}

template <typename T>
void QuadtraticEquationSolver<T>::solve_complete()
{
    int ea, eb, ec;
    T a2, b2, c2;
    a2 = frexp(a, &ea);
    b2 = frexp(b, &eb);
    c2 = frexp(c, &ec);
    int k = eb - ea;
    int l = ea - 2 * eb;
    int ecp = ec + l;
    int k1, k2;
    constexpr T two = 2;
    keep_exponent(k, k1, k2);
    constexpr int e_min = m_min + 2 * n_bit_f - 4;
    constexpr int e_max = m_max - 2 - (n_bit_f >> 1);
    if (e_min <= ecp && ecp < e_max)
    {
        T cp = c2 * std::pow(two, static_cast<T>(ecp));
        T delta = kahan_discriminant(a2, b2, cp);
        if (delta < 0)
        {
            no_root();
            return;
        }
        if (delta > 0)
        {
            T y1 = -(two * cp) / (b2 + static_cast<T>(sign(b)) * std::sqrt(delta));
            T y2 = -(b2 + static_cast<T>(sign(b)) * std::sqrt(delta)) / (two * a2);
            y1 = (y1 * std::pow(two, static_cast<T>(k2))) * std::pow(two, static_cast<T>(k1));
            y2 = (y2 * std::pow(two, static_cast<T>(k2))) * std::pow(two, static_cast<T>(k1));
            low_high_sort(y1, y2, x1, x2);
            state = TWO_REAL;
            return;
        }
        one_real(((-b2 / (two * a2)) * std::pow(two, static_cast<T>(k2))) * std::pow(two, static_cast<T>(k1)));
        return;
    }
    int dm = ecp & (~1);
    int m = dm >> 1;
    int e = ecp & 1;
    T c3 = c2 * std::pow(two, static_cast<T>(e));
    int dm1, dm2;
    if (ecp < e_min)
    {
        T y1 = -b2 / a2;
        T y2 = c3 / (a2 * y1);
        keep_exponent(dm + k, dm1, dm2);
        y1 = (y1 * std::pow(two, static_cast<T>(k2))) * std::pow(two, static_cast<T>(k1));
        y2 = (y2 * std::pow(two, static_cast<T>(dm2))) * std::pow(two, static_cast<T>(dm1));
        low_high_sort(y1, y2, x1, x2);
        state = TWO_REAL;
        return;
    }
    // ecp > e_max
    if (sign(a) == sign(c))
    {
        no_root(); // or complex root
        return;
    }
    keep_exponent(m + k, dm1, dm2);
    T s = std::sqrt(std::fabs(c3 / a2));
    x2 = (s * std::pow(two, static_cast<T>(dm2))) * std::pow(two, static_cast<T>(dm1));
    x1 = -x2;
    state = TWO_REAL;
}

template <typename T>
void QuadtraticEquationSolver<T>::solve()
{
    if (is_invalid_input(a) || is_invalid_input(b) || is_invalid_input(c))
    {
        invalid_input();
        return;
    }
    if (a == 0)
    {
        solve_linear();
        return;
    }
    if (b == 0)
    {
        solve_axx_plus_c();
        return;
    }
    if (c == 0)
    {
        solve_axx_plus_bx();
        return;
    }
    solve_complete();
}

template <typename T>
SolverState QuadtraticEquationSolver<T>::solve(T &r1, T &r2)
{
    solve();
    if (((TWO_REAL == state) && (is_invalid_input(x1) || is_invalid_input(x2))) || (ONE_REAL == state && is_invalid_input(x1)))
    {
        state = OVER_UNDER_FLOW;
    }
    r1 = x1;
    r2 = x2;
    return state;
}

template <typename T>
void QuadtraticEquationSolver<T>::reset(const T a, const T b, const T c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    state = UNCERTAIN;
    x1 = 0;
    x2 = 0;
}

template <typename T>
const std::string QuadtraticEquationSolver<T>::print_solver_state(SolverState s)
{
#define CASE_SOLVER_STATE(x) \
    case x:                  \
        return #x;           \
        break
    switch (s)
    {
        CASE_SOLVER_STATE(UNCERTAIN);
        CASE_SOLVER_STATE(INVALID_INPUT);
        CASE_SOLVER_STATE(ALL_REAL);
        CASE_SOLVER_STATE(NO_ROOT);
        CASE_SOLVER_STATE(ONE_REAL);
        CASE_SOLVER_STATE(TWO_REAL);
        CASE_SOLVER_STATE(OVER_UNDER_FLOW);
    default:
        break;
    }
    return "UNKNOWN_ERROR";
#undef CASE_SOLVER_STATE
}

template <typename T>
const std::string QuadtraticEquationSolver<T>::print_solver_state()
{
    return QuadtraticEquationSolver<T>::print_solver_state(this->state);
}

#undef sign
#undef is_invalid_input

#endif
