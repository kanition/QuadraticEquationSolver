#include <string>
#include "QuadraticEquationSolver.h"

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
