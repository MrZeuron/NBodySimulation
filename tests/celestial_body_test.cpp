#include "CelestialBody.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {

// Универсальная проверка завершает тест при обнаружении ошибки.
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

// Проверяет, что тело с неверными данными нельзя создать.
void requireInvalidBody(double mass, const VectorND& position,
                        const VectorND& velocity) {
    bool threw = false;
    try {
        CelestialBody body(mass, position, velocity);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    require(threw, "invalid celestial body parameter must be rejected");
}

} // namespace

// Создаёт корректное тело, затем проверяет чтение и сеттеры.
int main() {
    VectorND position = VectorND::zero3();
    position[0] = 10.0;

    VectorND velocity = VectorND::zero3();
    velocity[1] = 2.0;

    CelestialBody body(5.0, position, velocity);

    require(body.mass() == 5.0, "mass was not stored");
    require(body.position()[0] == 10.0, "position was not stored");
    require(body.velocity()[1] == 2.0, "velocity was not stored");

    body.setMass(7.5);
    body.setPosition(VectorND(3, 1.0));
    body.setVelocity(VectorND(3, -1.0));

    require(body.mass() == 7.5, "mass setter is incorrect");
    require(body.position()[2] == 1.0, "position setter is incorrect");
    require(body.velocity()[0] == -1.0, "velocity setter is incorrect");

    // Отдельные случаи проверяют все ограничения валидности.
    requireInvalidBody(0.0, VectorND::zero3(), VectorND::zero3());
    requireInvalidBody(-1.0, VectorND::zero3(), VectorND::zero3());
    requireInvalidBody(std::numeric_limits<double>::infinity(),
                       VectorND::zero3(), VectorND::zero3());
    requireInvalidBody(1.0, VectorND::zero3(), VectorND::zero4());

    VectorND invalidPosition = VectorND::zero3();
    invalidPosition[0] = std::numeric_limits<double>::quiet_NaN();
    requireInvalidBody(1.0, invalidPosition, VectorND::zero3());

    std::cout << "CelestialBody tests passed\n";
    return 0;
}
