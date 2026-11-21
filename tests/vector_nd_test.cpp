#include "VectorND.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace {

// Универсальная проверка: при false тест сразу завершается с объяснением.
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

// Проверяет отклонение недопустимых размерностей конструктором.
void requireThrowsInvalidDimension(std::size_t dimension) {
    bool threw = false;
    try {
        VectorND vector(dimension);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    require(threw, "invalid dimensions must be rejected");
}

} // namespace

// Последовательность проверяет основные операции и ошибки.
int main() {
    VectorND a = VectorND::zero3();
    a[0] = 1.0;
    a[1] = 2.0;
    a[2] = 2.0;

    require(a.dimension() == 3, "3D vector must contain three components");
    require(std::abs(a.norm() - 3.0) < 1e-12, "norm calculation is incorrect");
    require(std::abs(a.squaredNorm() - 9.0) < 1e-12, "squared norm calculation is incorrect");

    VectorND b(3, 2.0);
    VectorND sum = a + b;
    require(std::abs(sum[0] - 3.0) < 1e-12, "addition is incorrect");
    require(std::abs(a.dot(b) - 10.0) < 1e-12, "dot product is incorrect");

    VectorND scaled = a * 2.0;
    require(std::abs(scaled[1] - 4.0) < 1e-12, "scalar multiplication is incorrect");

    bool mismatchedDimensionThrew = false;
    try {
        (void)(a + VectorND::zero4());
    } catch (const std::invalid_argument&) {
        mismatchedDimensionThrew = true;
    }
    require(mismatchedDimensionThrew, "mismatched dimensions must be rejected");

    requireThrowsInvalidDimension(2);
    requireThrowsInvalidDimension(5);

    bool outOfRangeThrew = false;
    try {
        (void)a[3];
    } catch (const std::out_of_range&) {
        outOfRangeThrew = true;
    }
    require(outOfRangeThrew, "index outside the vector must be rejected");

    std::cout << "VectorND tests passed\n";
    return 0;
}
