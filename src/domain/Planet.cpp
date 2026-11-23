#include "Planet.h"
#include <cmath>
#include <stdexcept>

Planet::Planet(double mass, const VectorND& position, const VectorND& velocity,
               double radius, BodyColor color)
    : CelestialBody(mass, position, velocity), radius_(radius), color_(color) {
    if (!std::isfinite(radius) || radius <= 0.0
        || color.red < 0 || color.red > 255 || color.green < 0 || color.green > 255
        || color.blue < 0 || color.blue > 255) {
        throw std::invalid_argument("Invalid planet appearance parameters");
    }
}
Appearance Planet::appearance() const {
    Appearance result;
    result.symbol = 'P';
    result.radius = radius_;
    result.color = color_;
    return result;
}
const char* Planet::typeName() const {
    return "Planet";
}

std::unique_ptr<CelestialBody> Planet::clone() const {
    return std::make_unique<Planet>(*this);
}
