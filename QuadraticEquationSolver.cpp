#include <cmath>
#include "QuadraticEquationSolver.h"

constexpr double inf = std::numeric_limits<double>::infinity();
constexpr int n_bit_e = 11;
constexpr int n_bit_f = 52;
constexpr int m_max = (1 << (n_bit_e - 1)) - 1;
constexpr int m_min = 1 - m_max;

const std::string print_solver_state(SolverState s)
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

static int sign(const double x)
{
    if (x < 0)
    {
        return -1;
    }
    return 1;
}

static void low_high_sort(const double y1, const double y2, double &x1, double &x2)
{
    if (y1 < y2)
    {
        x1 = y1;
        x2 = y2;
    }
    else
    {
        x1 = y2;
        x2 = y1;
    }
}

static void keep_exponent(const int m, int &m1, int &m2)
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

static bool is_invalid_input(const double x)
{
    return std::isnan(x) || std::isinf(x);
}

static void veltkamp_split(const double x, double &xhigh, double &xlow)
{
    // split x = xhigh + xlow, and xhigh only uses high fraction bit, xlow only uses low fraction bits
    constexpr int coff = (1 << ((n_bit_f >> 1) + 1)) + 1;
    double gamma = coff * x;
    double delta = x - gamma;
    xhigh = gamma + delta;
    xlow = x - xhigh;
}

static double exactmult(const double x, const double y, const double pxy)
{
    double xhi, xlo, yhi, ylo;
    veltkamp_split(x, xhi, xlo);
    veltkamp_split(y, yhi, ylo);
    double t1 = -pxy + xhi * yhi;
    double t2 = t1 + xhi * ylo;
    double t3 = t2 + xlo * yhi;
    double e = t3 + xlo * ylo;
    return e;
}

static double kahan_discriminant(const double a, const double b, const double c)
{
    double p = b * b;
    double q = 4 * a * c;
    double d = p - q;
    if (3 * std::abs(d) >= (p + q))
    {
        // b*b and 4ac are different enough
        return d;
    }
    double dp = exactmult(b, b, p);
    double dq = exactmult(4 * a, c, q);
    d = d + (dp - dq);
    return d;
}

QuadtraticEquationSolver::QuadtraticEquationSolver(const double a, const double b, const double c)
    : a(a), b(b), c(c), state(UNCERTAIN), x1(0), x2(0)
{
    static_assert(std::numeric_limits<double>::is_iec559, "Not Support IEC 559 / IEEE 754 standard");
}

QuadtraticEquationSolver::~QuadtraticEquationSolver()
{
}

void QuadtraticEquationSolver::invalid_input()
{
    state = INVALID_INPUT;
}

void QuadtraticEquationSolver::all_real()
{
    state = ALL_REAL;
    x1 = inf;
    x2 = -inf;
}

void QuadtraticEquationSolver::no_root()
{
    state = NO_ROOT;
    x1 = std::nan("");
    x2 = std::nan("");
}

void QuadtraticEquationSolver::one_real(const double x)
{
    state = ONE_REAL;
    x1 = x;
    x2 = std::nan("");
}

void QuadtraticEquationSolver::solve_linear()
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

void QuadtraticEquationSolver::sqrt_minus_c_div_a()
{
    int ea, ec;
    double a2, c2;
    a2 = frexp(a, &ea);
    c2 = frexp(c, &ec);
    int ecp = ec - ea;
    int m = (ecp & (~1)) >> 1; // even number towards zero, then divided by 2
    double c3 = c2 * std::pow(2, ecp & 1);
    double s = std::sqrt(-c3 / a2);
    int m1, m2;
    keep_exponent(m, m1, m2);
    x2 = (s * std::pow(2, m2)) * std::pow(2, m1);
    x1 = -x2;
    state = TWO_REAL;
}

void QuadtraticEquationSolver::solve_axx_plus_c()
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

void QuadtraticEquationSolver::solve_axx_plus_bx()
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

void QuadtraticEquationSolver::solve_complete()
{
    int ea, eb, ec;
    double a2, b2, c2;
    a2 = frexp(a, &ea);
    b2 = frexp(b, &eb);
    c2 = frexp(c, &ec);
    int k = eb - ea;
    int l = ea - 2 * eb;
    int ecp = ec + l;
    int k1, k2;
    keep_exponent(k, k1, k2);
    constexpr int e_min = m_min + 2 * n_bit_f - 4;
    constexpr int e_max = m_max - 2 - (n_bit_f >> 1);
    if (e_min <= ecp && ecp < e_max)
    {
        double cp = c2 * std::pow(2, ecp);
        double delta = kahan_discriminant(a2, b2, cp);
        if (delta < 0)
        {
            x1 = std::nan("");
            x2 = std::nan("");
            state = NO_ROOT;
            return;
        }
        if (delta > 0)
        {
            double y1 = -(2 * cp) / (b2 + sign(b) * std::sqrt(delta));
            double y2 = -(b2 + sign(b) * std::sqrt(delta)) / (2 * a2);
            y1 = (y1 * std::pow(2, k2)) * std::pow(2, k1);
            y2 = (y2 * std::pow(2, k2)) * std::pow(2, k1);
            low_high_sort(y1, y2, x1, x2);
            state = TWO_REAL;
            return;
        }
        x1 = ((-b2 / (2 * a2)) * std::pow(2, k2)) * std::pow(2, k1);
        x2 = std::nan("");
        state = ONE_REAL;
        return;
    }
    int dm = ecp & (~1);
    int m = dm >> 1;
    int e = ecp & 1;
    double c3 = c2 * std::pow(2, e);
    int dm1, dm2;
    if (ecp < e_min)
    {
        double y1 = -b2 / a2;
        double y2 = c3 / (a2 * y1);
        keep_exponent(dm + k, dm1, dm2);
        y1 = (y1 * std::pow(2, k2)) * std::pow(2, k1);
        y2 = (y2 * std::pow(2, dm2)) * std::pow(2, dm1);
        low_high_sort(y1, y2, x1, x2);
        state = TWO_REAL;
        return;
    }
    // ecp > e_max
    if (sign(a) == sign(c))
    {
        x1 = std::nan("");
        x2 = std::nan("");
        state = NO_ROOT; // or complex root
        return;
    }
    keep_exponent(m + k, dm1, dm2);
    double s = std::sqrt(std::abs(c3 / a2));
    x2 = (s * std::pow(2, dm2)) * std::pow(2, dm1);
    x1 = -x2;
    state = TWO_REAL;
}

void QuadtraticEquationSolver::solve()
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

void QuadtraticEquationSolver::solve(SolverState &s, double &r1, double &r2)
{
    solve();
    if (((TWO_REAL == state) && (is_invalid_input(x1) || is_invalid_input(x2))) || (ONE_REAL == state && is_invalid_input(x1)))
    {
        state = OVER_UNDER_FLOW;
    }
    s = state;
    r1 = x1;
    r2 = x2;
}

void QuadtraticEquationSolver::reset(const double a, const double b, const double c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    state = UNCERTAIN;
    x1 = 0;
    x2 = 0;
}
