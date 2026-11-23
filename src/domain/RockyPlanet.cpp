#include "RockyPlanet.h"

RockyPlanet::RockyPlanet(double mass, const VectorND& position, const VectorND& velocity)
    : Planet(mass, position, velocity, 0.7, BodyColor{100, 170, 240}) {
}

Appearance RockyPlanet::appearance() const {
    Appearance result = Planet::appearance();
    result.symbol = 'P';
    result.surface = SurfaceStyle::Rocky;
    return result;
}

const char* RockyPlanet::typeName() const {
    return "RockyPlanet";
}

std::unique_ptr<CelestialBody> RockyPlanet::clone() const {
    return std::make_unique<RockyPlanet>(*this);
}
