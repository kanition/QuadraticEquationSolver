# Robustness & Precision

## Background
To solve the following quadratic equation with program
```math
ax^2+bx+c=0,
```
you might immediately recall this formula learned from your high school math textbook:
```math
\Delta = b^2-4ac
```
Then it's so easy to obtain the results:
```math
\begin{align*}
\text{No real root},\quad&\text{if}\quad\Delta < 0,\\
x_1=x_2=-\frac{b}{2a},\quad&\text{if}\quad\Delta = 0,\\
x_1,x_2=\frac{-b\pm\sqrt{\Delta}}{2a},\quad&\text{if}\quad\Delta > 0.
\end{align*}
```

So you implement it by just copying the formula.
You may even remember to use `double` to obtain better precision.

Then You test it with $a=1.0$, $b=4.0$, $c=-5.0$. That means you are going to solve
```math
x^2+4x-5=0.
```
The printed results are
```
Two real roots are found:
x_1 = -5,
x_2 = 1.
```
Everything is fine!
## Everything is fine …… really?
Let's solve another equation which is very similar:
```math
10^{-300}x^2+4\cdot 10^{-300}x-5\cdot 10^{-300}=0.
```
As intelligent as you are, you immediately recognize that it has **exactly the same** roots as the previous equation, because it can be re-written as
```math
10^{-300}(x^2+4x-5)=0.
```
However, I bet your program won't tell you that $x_1=-5$ and $x_2=1$.

In fact, the results are so surpring after setting $a=1e-300$, $b=4e-300$, $c=-5e-300$ :
```
One real root is found:
x_1 = -2.
```
Now I suggest to try another equation:
```math
10^{200}x^2+4\cdot 10^{200}x-5\cdot 10^{200}=0.
```
The roots should be the **same**, right? But you may get
```
Abnormal roots:
x_1 = -inf
x_2 = inf
```
## What happened!?
[*Floating-Point*](https://en.wikipedia.org/wiki/Floating-point_arithmetic) format has **limited** precision.
Arithmetic *underflow* and *overflows* may occur.
[*Catastrophic cancellation*](https://en.wikipedia.org/wiki/Catastrophic_cancellation) also disrupts your computation.
All these make the results wrong.
Solving such an equation is not as simple as you think.

## That's what our solver tries to solve!
We put the *naive* solver in [test.h](./test/test.h) for comparison.

For each format (single or double-precision),
41 cases are listed in [float_test.cpp](./test/float_test.cpp) and [double_test.cpp](./test/double_test.cpp) respectively.

Here are the results for `double_test`.
You can find that our solver has much better robustness and precision.
```
Double min: 2.22507e-308
Double max: 1.79769e+308
===================================================================
(1) a = b = c = 0
Solving: 0 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ALL_REAL          |          ALL_REAL          |
|  x_1  |            inf             |            inf             |
|  x_2  |            -inf            |            -inf            |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(2) a = b = 0, c != 0
Solving: 4 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(3) a = b = 0, c ≈ 0
Solving: 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(4) a = b = 0, |c| >> 0
Solving: 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(5) b != 0
Solving: 2 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(6) |b| ≈ 0
Solving: 2.22507e-308 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(7) |b| >> 0
Solving: 1.79769e+308 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(8) bx + c = 0
Solving: 2 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             -4             |             -4             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(9) bx + c = 0, |x| >> 0
Solving: 2e-307 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e+307           |          -4e+307           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(10) bx + c = 0, |x| >> 0
Solving: 2 * x + 8e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e+307           |          -4e+307           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(11) bx + c = 0, x ≈ 0
Solving: 2e+307 * x + 0.08 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e-309           |          -4e-309           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(12) bx + c = 0, x ≈ 0
Solving: 2 * x + 8e-309 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |          -4e-309           |          -4e-309           |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(13) bx + c = 0, overflow
Solving: 2.22507e-308 * x + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |      OVER_UNDER_FLOW       |      OVER_UNDER_FLOW       |
|  x_1  |            -inf            |            -inf            |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(14) bx + c = 0, underflow
Solving: 1.79769e+308 * x + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             -0             |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(15) ax^2 = 0
Solving: 5 * x^2 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             0              |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(16) ax^2 + c = 0, Δ < 0
Solving: 5 * x^2 + 7 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(17) ax^2 + c = 0, Δ < 0, |a| ≈ 0, |c| ≈ 0
Solving: 2.22507e-308 * x^2 + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          ONE_REAL          |
|  x_1  |            nan             |             -0             |
|  x_2  |            nan             |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(18) ax^2 + c = 0, Δ < 0, |a| >> 0, |c| >> 0
Solving: 1.79769e+308 * x^2 + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(19) ax^2 + c = 0, Δ < 0, |a| >> 0, |c| ≈ 0
Solving: 1.79769e+308 * x^2 + 2.22507e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(20) ax^2 + c = 0, Δ < 0, |a| ≈ 0, |c| >> 0
Solving: 2.22507e-308 * x^2 + 1.79769e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(21) ax^2 + c = 0, Δ > 0
Solving: 1 * x^2 - 9 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             -3             |             -3             |
|  x_2  |             3              |             3              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(22) ax^2 + c = 0, Δ > 0, avoid overflow
Solving: 1e+307 * x^2 - 9e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             -3             |            -inf            |
|  x_2  |             3              |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(23) ax^2 + c = 0, Δ > 0, avoid underflow
Solving: 1e-309 * x^2 - 9e-309 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |             -3             |             -0             |
|  x_2  |             3              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(24) ax^2 + c = 0, Δ > 0, avoid overflow
Solving: 1e-309 * x^2 - 2.89e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |         -1.7e+308          |         -1.7e+308          |
|  x_2  |          1.7e+308          |          1.7e+308          |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(25) ax^2 + c = 0, Δ > 0, avoid underflow
Solving: 1e+308 * x^2 - 2.89e-308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |         -1.7e-308          |         -nan(ind)          |
|  x_2  |          1.7e-308          |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(26) ax^2 + bx = 0, exist x = 0
Solving: 4 * x^2 + 8 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             -2             |             -2             |
|  x_2  |             0              |             0              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(27) ax^2 + bx = 0, exist x = 0
Solving: 4 * x^2 - 8 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |             0              |             0              |
|  x_2  |             2              |             2              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(28) ax^2 + bx = 0, exist x = 0, avoid overflow
Solving: 4e+307 * x^2 - 8e+307 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             0              |            -inf            |
|  x_2  |             2              |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(29) ax^2 + bx = 0, exist x = 0, avoid underflow
Solving: 4e-308 * x^2 - 8e-308 * x = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |             0              |             1              |
|  x_2  |             2              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(30) ax^2 + bx + c = 0, Δ < 0
Solving: 2 * x^2 + 8 * x + 10 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          NO_ROOT           |
|  x_1  |            nan             |            nan             |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(31) ax^2 + bx + c = 0, Δ < 0, avoid underflow
Solving: 2e-308 * x^2 + 8e-308 * x + 1e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |          ONE_REAL          |
|  x_1  |            nan             |             -2             |
|  x_2  |            nan             |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(32) ax^2 + bx + c = 0, Δ < 0, avoid underflow
Solving: 2e+307 * x^2 + 8e+307 * x + 1e+308 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          NO_ROOT           |      OVER_UNDER_FLOW       |
|  x_1  |            nan             |         -nan(ind)          |
|  x_2  |            nan             |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(33) ax^2 + bx + c = 0, Δ = 0
Solving: 2 * x^2 - 8 * x + 8 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             2              |             2              |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(34) ax^2 + bx + c = 0, Δ = 0, avoid overflow
Solving: 2e+307 * x^2 - 8e+307 * x + 8e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |             2              |         -nan(ind)          |
|  x_2  |            nan             |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(35) ax^2 + bx + c = 0, Δ = 0, avoid underflow
Solving: 3e-308 * x^2 - 1.2e-307 * x + 1.2e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          ONE_REAL          |          ONE_REAL          |
|  x_1  |             2              |             2              |
|  x_2  |            nan             |            nan             |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(36) ax^2 + bx + c = 0, Δ > 0
Solving: 6 * x^2 - 33 * x + 45 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |            2.5             |            2.5             |
|  x_2  |             3              |             3              |
|  gap  |                          Same                           |
+-------+----------------------------+----------------------------+

(37) ax^2 + bx + c = 0, Δ > 0, avoid overflow
Solving: 6e+306 * x^2 - 3.3e+307 * x + 4.5e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |            2.5             |         -nan(ind)          |
|  x_2  |             3              |         -nan(ind)          |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(38) ax^2 + bx + c = 0, Δ > 0, avoid underflow
Solving: 6e-308 * x^2 - 3.3e-307 * x + 4.5e-307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          ONE_REAL          |
|  x_1  |            2.5             |            2.75            |
|  x_2  |             3              |            nan             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(39) Golden Ratio is 0.618033988749894(8)
Solving: 1 * x^2 + 1 * x - 1 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |          TWO_REAL          |
|  x_1  |     -1.61803398874989      |     -1.61803398874989      |
|  x_2  |     0.618033988749895      |     0.618033988749895      |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(40) Golden Ratio is 0.618033988749894(8), avoid overflow
Solving: 1e+307 * x^2 + 1e+307 * x - 1e+307 = 0
+-------+----------------------------+----------------------------+
|double |  QuadtraticEquationSolver  |        Naive Solver        |
+-------+----------------------------+----------------------------+
| state |          TWO_REAL          |      OVER_UNDER_FLOW       |
|  x_1  |     -1.61803398874989      |            -inf            |
|  x_2  |     0.618033988749895      |            inf             |
|  gap  |                        Different                        |
+-------+----------------------------+----------------------------+

(41) Golden Ratio is 0.618033988749894(8), avoid underflow
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
