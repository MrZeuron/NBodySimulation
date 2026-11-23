#include "GasGiant.h"

GasGiant::GasGiant(double mass, const VectorND& position, const VectorND& velocity)
    : Planet(mass, position, velocity, 1.4, BodyColor{210, 170, 110}) {
}

Appearance GasGiant::appearance() const {
    Appearance result = Planet::appearance();
    result.symbol = 'G';
    result.surface = SurfaceStyle::Banded;
    return result;
}

const char* GasGiant::typeName() const {
    return "GasGiant";
}

std::unique_ptr<CelestialBody> GasGiant::clone() const {
    return std::make_unique<GasGiant>(*this);
}
