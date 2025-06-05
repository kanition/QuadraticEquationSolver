# Robust High-Precision Quadratic Equation Solver

A *Robust High-Precision* Quadratic Equation Solver in C++20 for
```math
ax^2+bx+c=0,
```
where $a,b,c$ and $x$ are all real numbers in the [*Floating-Point*](https://en.wikipedia.org/wiki/Floating-point_arithmetic) format,
under [IEEE Standard for Floating-Point Arithmetic (IEEE 754)](https://en.wikipedia.org/wiki/IEEE_754).

Both single-precision (32-bits) and double-precision (64-bits) are supported.

## Quick Start
To use this solver in your project, you can:
1. Include the header file [QuadraticEquationSolver.h](./QuadraticEquationSolver.h);
2. Initialize a `QuadraticEquationSolver` with $a,b,c$;
3. Call the solver method `solve` to get the roots and state!
```cpp
#include "QuadraticEquationSolver.h"

double a = 1.0, b = 4.0, c = -5.0;
QuadtraticEquationSolver<double> solver(a, b, c);

double x1, x2;
// x1 and x2 will be the two roots! And s is their state.
SolverState s = solver.solve(x1, x2);
```

4. Print the roots and state;
```cpp
#include <iostream>

std::cout << "x1: " << x1 << std::endl;
std::cout << "x2: " << x2 << std::endl;

// Print the state of the solver itself
std::cout << "State: " << solver.print_solver_state() << std::endl;

// Or print the specified state `s` by the class static method
std::cout << "State: " << QuadtraticEquationSolver<double>::print_solver_state(s) << std::endl;
```

5. You can re-use the solver by `reset`:
```cpp
a = 1.0, b = 4.0, c = 4.0;
solver.reset(a, b, c); // Clean the solver state and reset a, b, c
s = solver.solve(x1, x2); // Solve the new equation
```

## Compile and Run Demo
Requirements:
* [CMake](https://cmake.org/) >= 3.20
* C++ compiler [supporting C++20 standard](https://en.cppreference.com/w/cpp/compiler_support.html#C.2B.2B20_features).
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
## Robustness & Precision
See [here](./Robustness_Precision.md) for more detailed discussion and surprising cases.

## Acknowledgement
Thank the author for developing and sharing this algorithm in the following paper.
```
Frédéric Goualard. The Ins and Outs of Solving Quadratic Equations with Floating-Point Arithmetic. 2023. ⟨hal-04116310⟩
```
### Citation
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

## LICENSE

Copyright 2025 Kanition

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

