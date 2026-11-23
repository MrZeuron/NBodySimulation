#include "RedGiant.h"

RedGiant::RedGiant(double mass, const VectorND& position, const VectorND& velocity)
    : Star(mass, position, velocity, 2.5, 4.0, 3500.0) {
}

Appearance RedGiant::appearance() const {
    Appearance result = Star::appearance();
    result.symbol = 'R';
    return result;
}

const char* RedGiant::typeName() const {
    return "RedGiant";
}

std::unique_ptr<CelestialBody> RedGiant::clone() const {
    return std::make_unique<RedGiant>(*this);
}
