#pragma once

#include <iostream>

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

const std::string print_solver_state(SolverState s);

class QuadtraticEquationSolver
{
public:
	QuadtraticEquationSolver(const double a, const double b, const double c);
	~QuadtraticEquationSolver();
	void solve(SolverState &s, double &r1, double &r2);
	void reset(const double a, const double b, const double c);

private:
	double a;
	double b;
	double c;
	double x1;
	double x2;
	SolverState state;
	void invalid_input();
	void solve();
	void all_real();
	void no_root();
	void one_real(const double x);
	void solve_linear();
	void sqrt_minus_c_div_a();
	void solve_axx_plus_c();
	void solve_axx_plus_bx();
	void solve_complete();
};
