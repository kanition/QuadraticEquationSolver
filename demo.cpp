#include <iostream>
#include "QuadraticEquationSolver.h"

int main()
{
    // Equation 1
    double a = 1, b = 4, c = -5;
    double x1 = 0, x2 = 0;
    SolverState s;
    QuadtraticEquationSolver<double> solver(a, b, c);
    s = solver.solve(x1, x2);
    std::cout << "Solve: " << a << " * x^2"
              << (b < 0 ? " " : " +") << b << " * x"
              << (c < 0 ? " " : " +") << c << " = 0" << std::endl;
    std::cout << "State: " << QuadtraticEquationSolver<double>::print_solver_state(s) << std::endl;
    std::cout << "State: " << solver.print_solver_state() << std::endl;
    std::cout << "x1: " << x1 << std::endl;
    std::cout << "x2: " << x2 << std::endl
              << std::endl;

    // Equation 2
    a = 1, b = 4, c = 4;
    solver.reset(a, b, c); // Clean the solver state and reset a, b, c
    s = solver.solve(x1, x2);
    std::cout << "Solve: " << a << " * x^2"
              << (b < 0 ? " " : " +") << b << " * x"
              << (c < 0 ? " " : " +") << c << " = 0" << std::endl;
    std::cout << "State: " << QuadtraticEquationSolver<double>::print_solver_state(s) << std::endl;
    std::cout << "State: " << solver.print_solver_state() << std::endl;
    std::cout << "x1: " << x1 << std::endl;
    std::cout << "x2: " << x2 << std::endl
              << std::endl;
    return 0;
}