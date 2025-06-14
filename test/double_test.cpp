#include "test/test.h"

void test_double()
{
    constexpr double p = std::numeric_limits<double>::min();
    constexpr double q = std::numeric_limits<double>::max();
    std::cout << "Double min: " << p << std::endl;
    std::cout << "Double max: " << q << std::endl;
    std::cout << std::string(67, '=') << std::endl;

    demo("a = b = c = 0", 0., 0., 0.);
    demo("a = b = 0, c != 0", 0., 0., 4.);
    demo("a = b = 0, c ~= 0", 0., 0., p);
    demo("a = b = 0, |c| >> 0", 0., 0., q);
    demo("b != 0", 0., 2., 0.);
    demo("|b| ~= 0", 0., p, 0.);
    demo("|b| >> 0", 0., q, 0.);

    demo("bx + c = 0", 0., 2., 8.);
    demo("bx + c = 0, |x| >> 0", 0., 2e-307, 8.);
    demo("bx + c = 0, |x| >> 0", 0., 2., 8e307);
    demo("bx + c = 0, x ~= 0", 0., 2e307, 8e-2);
    demo("bx + c = 0, x ~= 0", 0., 2., 8e-309);
    demo("bx + c = 0, overflow", 0., p, q);
    demo("bx + c = 0, underflow", 0., q, p);

    demo("ax^2 = 0", 5., 0., 0.);
    demo("ax^2 + c = 0, Δ < 0", 5., 0., 7.);
    demo("ax^2 + c = 0, Δ < 0, |a| ~= 0, |c| ~= 0", p, 0., p);
    demo("ax^2 + c = 0, Δ < 0, |a| >> 0, |c| >> 0", q, 0., q);
    demo("ax^2 + c = 0, Δ < 0, |a| >> 0, |c| ~= 0", q, 0., p);
    demo("ax^2 + c = 0, Δ < 0, |a| ~= 0, |c| >> 0", p, 0., q);

    demo("ax^2 + c = 0, Δ > 0", 1., 0., -9.);
    demo("ax^2 + c = 0, Δ > 0, avoid overflow", 1e307, 0., -9e307);
    demo("ax^2 + c = 0, Δ > 0, avoid underflow", 1e-309, 0., -9e-309);
    demo("ax^2 + c = 0, Δ > 0, avoid overflow", 1e-309, 0., -2.89e307);
    demo("ax^2 + c = 0, Δ > 0, avoid underflow", 1e308, 0., -2.89e-308);

    demo("ax^2 + bx = 0, exist x = 0", 4., 8., 0.);
    demo("ax^2 + bx = 0, exist x = 0", 4., -8., 0.);
    demo("ax^2 + bx = 0, exist x = 0, avoid overflow", 4e307, -8e307, 0.);
    demo("ax^2 + bx = 0, exist x = 0, avoid underflow", 4e-308, -8e-308, 0.);

    demo("ax^2 + bx + c = 0, Δ < 0", 2., 8., 10.);
    demo("ax^2 + bx + c = 0, Δ < 0, avoid underflow", 2e-308, 8e-308, 1e-307);
    demo("ax^2 + bx + c = 0, Δ < 0, avoid underflow", 2e307, 8e307, 1e308);

    demo("ax^2 + bx + c = 0, Δ = 0", 2., -8., 8.);
    demo("ax^2 + bx + c = 0, Δ = 0, avoid overflow", 2e307, -8e307, 8e307);
    demo("ax^2 + bx + c = 0, Δ = 0, avoid underflow", 3e-308, -1.2e-307, 1.2e-307);

    demo("ax^2 + bx + c = 0, Δ > 0", 6., -33., 45.);
    demo("ax^2 + bx + c = 0, Δ > 0, avoid overflow", 6e306, -3.3e307, 4.5e307);
    demo("ax^2 + bx + c = 0, Δ > 0, avoid underflow", 6e-308, -3.3e-307, 4.5e-307);

    demo("Golden Ratio is 0.618033988749894(8)", 1., 1., -1.);
    demo("Golden Ratio is 0.618033988749894(8), avoid overflow", 1.e307, 1.e307, -1.e307);
    demo("Golden Ratio is 0.618033988749894(8), avoid underflow", 1.e-307, 1.e-307, -1.e-307);
}

int main()
{
    test_double();
    return 0;
}