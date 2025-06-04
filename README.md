# Robust High-Precision Quadratic Equation Solver

A *Robust High-Precision* Quadratic Equation Solver in C++20 for
```math
ax^2+bx+c=0,
```
where $a,b,c$ and $x$ are all in [*Floating-Point*](https://en.wikipedia.org/wiki/Floating-point_arithmetic) format,
under [IEEE Standard for Floating-Point Arithmetic (IEEE 754)](https://en.wikipedia.org/wiki/IEEE_754).

Support both single-precision (32-bits) and double-precision (64-bits).

## Quick Start
To use this solver in your project, you can:
1. Include the header file [QuadraticEquationSolver.h](./QuadraticEquationSolver.h);
2. Initialize a `QuadraticEquationSolver` with $a,b,c$;
3. Set a `SolverState` variable;
4. Call the solver method `solve` to get the roots and state!
```cpp
#include "QuadraticEquationSolver.h"

double a = 1, b = 4, c = -5;
QuadtraticEquationSolver<double> solver(a, b, c);

SolverState s;
double x1, x2;
// x1 and x2 will be the two roots! And s means their state.
s = solver.solve(x1, x2);
```

You can print the roots and state like this:
```cpp
#include <iostream>

// Print the two roots
std::cout << "x1: " << x1 << std::endl;
std::cout << "x2: " << x2 << std::endl;

// Print the state of the solver itself
std::cout << "State: " << solver.print_solver_state() << std::endl;

// Or print the specified state `s` by the class static method
std::cout << "State: " << QuadtraticEquationSolver<double>::print_solver_state(s) << std::endl;
```

And you can re-use the solver by `reset`
```cpp
a = 1, b = 4, c = 4;
solver.reset(a, b, c); // Clean the solver state and reset a, b, c
s = solver.solve(x1, x2); // Solve the new equation
```

## Compile and Run Demo
You need to install [CMake](https://cmake.org/) >= 3.20 and
a proper compiler [supporting C++20 standard](https://en.cppreference.com/w/cpp/compiler_support.html#C.2B.2B20_features).

Then just run these commands
```bash
# Make a directory OUTSIDE this source directory and go into it
mkdir ../build
cd ../build

# Use cmake to configure the project with this SOURCE DIRECTORY
cmake ../QuadtraticEquationSolver

# Compile
make

# Run the demo
./demo

# Optional: run the test
make test

# Optional: Compare QuadtraticEquationSolver and a naive non-robust solver
./float_test  # For 41 single-precision (32-bits) cases
./double_test # For 41 double-precision (64-bits) cases
```

## About Algorithm Author
Thank the author for developing and sharing this algorithm in the following paper.
```
Frédéric Goualard. The Ins and Outs of Solving Quadratic Equations with Floating-Point Arithmetic. 2023. ⟨hal-04116310⟩
```
For bibtex
```bibtex
@unpublished{goualard:hal-04116310,
  TITLE = {{The Ins and Outs of Solving Quadratic Equations with Floating-Point Arithmetic}},
  AUTHOR = {Goualard, Fr{\'e}d{\'e}ric},
  URL = {https://cnrs.hal.science/hal-04116310},
  NOTE = {working paper or preprint},
  YEAR = {2023},
  MONTH = Jun,
  KEYWORDS = {Floating-point arithmetic ; numerical errors ; robustness ; quadratic equation},
  PDF = {https://cnrs.hal.science/hal-04116310v1/file/quadratics.pdf},
  HAL_ID = {hal-04116310},
  HAL_VERSION = {v1},
}
```
## Robustness & Precision
There are 41 cases for each format (single or double-precision).
Let's take "double" as an example here.

To show the robustness and precision of our solver,
we implement another naive solver for comparison through
```math
\Delta = b^2-4ac
```
and
```math
\begin{align*}
\text{No real root},\quad&\text{if}\quad\Delta < 0,\\
x_1=x_2=-\frac{b}{2a},\quad&\text{if}\quad\Delta = 0,\\
x_1,x_2=\frac{-b\pm\sqrt{\Delta}}{2a},\quad&\text{if}\quad\Delta > 0.
\end{align*}
```
Although the naive solver has considered that the equations may degenerate into a linear equation, it still gives wrong results for some hard cases.

We list all cases here for reference.
```
Double min: 2.22507e-308
Double max: 1.79769e+308
===================================================================
a = b = c = 0
Solving: 0= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ALL_REAL          |          ALL_REAL          |
|  x_1  |            inf             |            inf             |
|  x_2  |            -inf            |            -inf            |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

a = b = 0, c != 0
Solving: 4 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

a = b = 0, c ≈ 0
Solving: 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

a = b = 0, |c| >> 0
Solving: 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

b != 0
Solving: 2 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

|b| ≈ 0
Solving: 2.22507e-308 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

|b| >> 0
Solving: 1.79769e+308 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0
Solving: 2 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             -4             |             -4             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, |x| >> 0
Solving: 2e-307 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e+307           |          -4e+307           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, |x| >> 0
Solving: 2 * x + 8e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e+307           |          -4e+307           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, x ≈ 0
Solving: 2e+307 * x + 0.08 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e-309           |          -4e-309           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, x ≈ 0
Solving: 2 * x + 8e-309 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e-309           |          -4e-309           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, overflow
Solving: 2.22507e-308 * x + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |      OVER_UNDER_FLOW       |      OVER_UNDER_FLOW       |
|  x_1  |            -inf            |            -inf            |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

bx + c = 0, underflow
Solving: 1.79769e+308 * x + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             -0             |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 = 0
Solving: 5 * x^2= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ < 0
Solving: 5 * x^2 + 7 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ < 0, |a| ≈ 0, |c| ≈ 0
Solving: 2.22507e-308 * x^2 + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          ONE_REAL          |
|  x_1  |            nan             |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ < 0, |a| >> 0, |c| >> 0
Solving: 1.79769e+308 * x^2 + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ < 0, |a| >> 0, |c| ≈ 0
Solving: 1.79769e+308 * x^2 + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ < 0, |a| ≈ 0, |c| >> 0
Solving: 2.22507e-308 * x^2 + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ > 0
Solving: 1 * x^2 - 9 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             -3             |             -3             |
|  x_2  |             3              |             3              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ > 0, avoid overflow
Solving: 1e+307 * x^2 - 9e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             -3             |            -inf            |
|  x_2  |             3              |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ > 0, avoid underflow
Solving: 1e-309 * x^2 - 9e-309 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |             -3             |             -0             |
|  x_2  |             3              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ > 0, avoid overflow
Solving: 1e-309 * x^2 - 2.89e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |         -1.7e+308          |         -1.7e+308          |
|  x_2  |          1.7e+308          |          1.7e+308          |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + c = 0, Δ > 0, avoid underflow
Solving: 1e+308 * x^2 - 2.89e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |         -1.7e-308          |         -nan(ind)          |
|  x_2  |          1.7e-308          |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx = 0, exist x = 0
Solving: 4 * x^2 + 8 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             -2             |             -2             |
|  x_2  |             0              |             0              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx = 0, exist x = 0
Solving: 4 * x^2 - 8 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             0              |             0              |
|  x_2  |             2              |             2              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx = 0, exist x = 0, avoid overflow
Solving: 4e+307 * x^2 - 8e+307 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             0              |            -inf            |
|  x_2  |             2              |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx = 0, exist x = 0, avoid underflow
Solving: 4e-308 * x^2 - 8e-308 * x= 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |             0              |             1              |
|  x_2  |             2              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ < 0
Solving: 2 * x^2 + 8 * x + 10 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ < 0, avoid underflow
Solving: 2e-308 * x^2 + 8e-308 * x + 1e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          ONE_REAL          |
|  x_1  |            nan             |             -2             |
|  x_2  |            nan             |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ < 0, avoid underflow
Solving: 2e+307 * x^2 + 8e+307 * x + 1e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |      OVER_UNDER_FLOW       |
|  x_1  |            nan             |         -nan(ind)          |
|  x_2  |            nan             |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ = 0
Solving: 2 * x^2 - 8 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             2              |             2              |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ = 0, avoid overflow
Solving: 2e+307 * x^2 - 8e+307 * x + 8e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             2              |         -nan(ind)          |
|  x_2  |            nan             |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ = 0, avoid underflow
Solving: 3e-308 * x^2 - 1.2e-307 * x + 1.2e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             2              |             2              |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ > 0
Solving: 6 * x^2 - 33 * x + 45 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |            2.5             |            2.5             |
|  x_2  |             3              |             3              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ > 0, avoid overflow
Solving: 6e+306 * x^2 - 3.3e+307 * x + 4.5e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |            2.5             |         -nan(ind)          |
|  x_2  |             3              |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

ax^2 + bx + c = 0, Δ > 0, avoid underflow
Solving: 6e-308 * x^2 - 3.3e-307 * x + 4.5e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |            2.5             |            2.75            |
|  x_2  |             3              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

Golden Ratio is 0.618033988749894(8)
Solving: 1 * x^2 + 1 * x - 1 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |     -1.61803398874989      |     -1.61803398874989      |
|  x_2  |     0.618033988749895      |     0.618033988749895      |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

Golden Ratio is 0.618033988749894(8), avoid overflow
Solving: 1e+307 * x^2 + 1e+307 * x - 1e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |     -1.61803398874989      |            -inf            |
|  x_2  |     0.618033988749895      |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

Golden Ratio is 0.618033988749894(8), avoid underflow
Solving: 1e-307 * x^2 + 1e-307 * x - 1e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |     -1.61803398874989      |            -0.5            |
|  x_2  |     0.618033988749895      |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+
```
