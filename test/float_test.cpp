#include "test/test.h"

void test_float()
{
    constexpr float p = std::numeric_limits<float>::min();
    constexpr float q = std::numeric_limits<float>::max();
    std::cout << "Float min: " << p << std::endl;
    std::cout << "Float max: " << q << std::endl;
    std::cout << std::string(67, '=') << std::endl;

    demo("a = b = c = 0", 0.f, 0.f, 0.f);
    demo("a = b = 0, c != 0", 0.f, 0.f, 4.f);
    demo("a = b = 0 != c ~= 0", 0.f, 0.f, p);
    demo("a = b = 0, c >> 0", 0.f, 0.f, q);
    demo("b != 0", 0.f, 2.f, 0.f);
    demo("b ~= 0", 0.f, p, 0.f);
    demo("b >> 0", 0.f, q, 0.f);

    demo("bx + c = 0", 0.f, 2.f, 8.f);
    demo("bx + c = 0, |x| >> 0", 0.f, 2e-37f, 8.f);
    demo("bx + c = 0, |x| >> 0", 0.f, 2.f, 8e37f);
    demo("bx + c = 0, x ~= 0", 0.f, 2e30f, 8e-7f);
    demo("bx + c = 0, x ~= 0", 0.f, 2.f, 8e-37f);
    demo("bx + c = 0, overflow", 0.f, p, q);
    demo("bx + c = 0, underflow", 0.f, q, p);

    demo("ax^2 = 0", 5.f, 0.f, 0.f);
    demo("ax^2 + c = 0, Δ < 0", 5.f, 0.f, 7.f);
    demo("ax^2 + c = 0, Δ < 0, |a| ~= 0, |c| ~= 0", p, 0.f, p);
    demo("ax^2 + c = 0, Δ < 0, |a| >> 0, |c| >> 0", q, 0.f, q);
    demo("ax^2 + c = 0, Δ < 0, |a| >> 0, |c| ~= 0", q, 0.f, p);
    demo("ax^2 + c = 0, Δ < 0, |a| ~= 0, |c| >> 0", p, 0.f, q);

    demo("ax^2 + c = 0, Δ > 0", 1.f, 0.f, -9.f);
    demo("ax^2 + c = 0, Δ > 0, avoid overflow", 1e37f, 0.f, -9e37f);
    demo("ax^2 + c = 0, Δ > 0, avoid underflow", 1e-37f, 0.f, -9e-37f);
    demo("ax^2 + c = 0, Δ > 0, avoid overflow", 1e-37f, 0.f, -2.89e37f);
    demo("ax^2 + c = 0, Δ > 0, avoid underflow", 1e37f, 0.f, -2.89e-37f);

    demo("ax^2 + bx = 0, exist x = 0", 4.f, 8.f, 0.f);
    demo("ax^2 + bx = 0, exist x = 0", 4.f, -8.f, 0.f);
    demo("ax^2 + bx = 0, exist x = 0, avoid overflow", 4e37f, -8e37f, 0.f);
    demo("ax^2 + bx = 0, exist x = 0, avoid underflow", 4e-37f, -8e-37f, 0.f);

    demo("ax^2 + bx + c = 0, Δ < 0", 2.f, 8.f, 10.f);
    demo("ax^2 + bx + c = 0, Δ < 0, avoid underflow", 2e-38f, 8e-38f, 1e-37f);
    demo("ax^2 + bx + c = 0, Δ < 0, avoid underflow", 2e37f, 8e37f, 1e38f);

    demo("ax^2 + bx + c = 0, Δ = 0", 2.f, -8.f, 8.f);
    demo("ax^2 + bx + c = 0, Δ = 0, avoid overflow", 2e37f, -8e37f, 8e37f);
    demo("ax^2 + bx + c = 0, Δ = 0, avoid underflow", 3e-38f, -1.2e-37f, 1.2e-37f);

    demo("ax^2 + bx + c = 0, Δ > 0", 6.f, -33.f, 45.f);
    demo("ax^2 + bx + c = 0, Δ > 0, avoid overflow", 6e36f, -3.3e37f, 4.5e37f);
    demo("ax^2 + bx + c = 0, Δ > 0, avoid underflow", 6e-38f, -3.3e-37f, 4.5e-37f);

    demo("Golden Ratio is 0.6180339(8)", 1.f, 1.f, -1.f);
    demo("Golden Ratio is 0.6180339(8), avoid overflow", 1.e37f, 1.e37f, -1.e37f);
    demo("Golden Ratio is 0.6180339(8), avoid underflow", 1.e-37f, 1.e-37f, -1.e-37f);
}

int main()
{
    test_float();
    return 0;
}