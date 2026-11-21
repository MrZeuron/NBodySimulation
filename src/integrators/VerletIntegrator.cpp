#include "VerletIntegrator.h"

std::vector<CelestialBody> VerletIntegrator::step(
    const std::vector<CelestialBody>& bodies,
    const GravityModel& gravity,
    double timeStep) const {
    requireValidStep(timeStep);

    std::vector<VectorND> oldAccelerations = accelerationsOf(bodies, gravity);
    std::vector<CelestialBody> predicted;

    // Сначала вычисляем все новые положения из одного исходного состояния.
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        VectorND position = bodies[i].position()
                            + bodies[i].velocity() * timeStep
                            + oldAccelerations[i] * (timeStep * timeStep / 2.0);
        predicted.push_back(CelestialBody(
            bodies[i].mass(), position, bodies[i].velocity()));
    }

    std::vector<VectorND> newAccelerations = accelerationsOf(predicted, gravity);
    std::vector<CelestialBody> result;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        VectorND velocity = bodies[i].velocity()
                            + (oldAccelerations[i] + newAccelerations[i])
                            * (timeStep / 2.0);
        result.push_back(CelestialBody(
            bodies[i].mass(), predicted[i].position(), velocity));
    }
    return result;
}
