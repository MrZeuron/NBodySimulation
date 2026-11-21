#include "Integrator.h"

#include <cmath>
#include <stdexcept>

Integrator::~Integrator() {
}

void Integrator::requireValidStep(double timeStep) const {
    if (!std::isfinite(timeStep) || timeStep <= 0.0) {
        throw std::invalid_argument("Time step must be finite and positive");
    }
}

std::vector<VectorND> Integrator::accelerationsOf(
    const std::vector<CelestialBody>& bodies,
    const GravityModel& gravity) const {
    std::vector<VectorND> accelerations = gravity.calculateAccelerations(bodies);
    if (accelerations.size() != bodies.size()) {
        throw std::invalid_argument("Acceleration count does not match body count");
    }
    return accelerations;
}
