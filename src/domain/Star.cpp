#include "Star.h"
#include <cmath>
#include <stdexcept>

Star::Star(double mass, const VectorND& position, const VectorND& velocity,
           double radius, double luminosity, double temperature)
    : CelestialBody(mass, position, velocity), radius_(radius),
      luminosity_(luminosity), temperature_(temperature) {
    if (!std::isfinite(radius) || radius <= 0.0 || !std::isfinite(luminosity)
        || luminosity < 0.0 || !std::isfinite(temperature) || temperature <= 0.0) {
        throw std::invalid_argument("Invalid star appearance parameters");
    }
}
double Star::luminosity() const {
    return luminosity_;
}

double Star::temperature() const {
    return temperature_;
}

Appearance Star::appearance() const {
    Appearance result;
    result.symbol = 'S';
    result.radius = radius_;
    result.glow = std::log1p(luminosity_);
    result.color = {255, 230, 160};
    if (temperature_ < 4500.0) {
        result.color = {255, 100, 50};
    } else if (temperature_ > 7500.0) {
        result.color = {170, 210, 255};
    }
    return result;
}
const char* Star::typeName() const {
    return "Star";
}

std::unique_ptr<CelestialBody> Star::clone() const {
    return std::make_unique<Star>(*this);
}
