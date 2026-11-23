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


std::vector<VectorND> Integrator::velocitiesOf(
    const std::vector<CelestialBody>& bodies) const {
    std::vector<VectorND> velocities;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        velocities.push_back(bodies[i].velocity());
    }
    return velocities;
}

std::vector<CelestialBody> Integrator::makeStage(
    const std::vector<CelestialBody>& bodies,
    const std::vector<VectorND>& positionSlopes,
    const std::vector<VectorND>& velocitySlopes,
    double stageStep) const {
    if (positionSlopes.size() != bodies.size() ||
        velocitySlopes.size() != bodies.size()) {
        throw std::invalid_argument("Stage data does not match body count");
    }

    std::vector<CelestialBody> stage;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        VectorND position = bodies[i].position() + positionSlopes[i] * stageStep;
        VectorND velocity = bodies[i].velocity() + velocitySlopes[i] * stageStep;
        stage.push_back(CelestialBody(bodies[i].mass(), position, velocity));
    }
    return stage;
}
