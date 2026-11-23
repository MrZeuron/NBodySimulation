#include "Asteroid.h"

Asteroid::Asteroid(double mass, const VectorND& position, const VectorND& velocity)
    : CelestialBody(mass, position, velocity) {
}

Appearance Asteroid::appearance() const {
    Appearance result = CelestialBody::appearance();
    result.symbol = 'a';
    result.surface = SurfaceStyle::Irregular;
    result.radius = 0.2;
    result.color = {150, 120, 95};
    return result;
}

const char* Asteroid::typeName() const {
    return "Asteroid";
}

std::unique_ptr<CelestialBody> Asteroid::clone() const {
    return std::make_unique<Asteroid>(*this);
}
