#include "Moon.h"

Moon::Moon(double mass, const VectorND& position, const VectorND& velocity)
    : CelestialBody(mass, position, velocity) {
}

Appearance Moon::appearance() const {
    Appearance result = CelestialBody::appearance();
    result.symbol = 'm';
    result.surface = SurfaceStyle::Cratered;
    result.radius = 0.35;
    result.color = {160, 160, 170};
    return result;
}

const char* Moon::typeName() const {
    return "Moon";
}

std::unique_ptr<CelestialBody> Moon::clone() const {
    return std::make_unique<Moon>(*this);
}
