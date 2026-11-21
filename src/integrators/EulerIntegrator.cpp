#include "EulerIntegrator.h"

std::vector<CelestialBody> EulerIntegrator::step(
    const std::vector<CelestialBody>& bodies,
    const GravityModel& gravity,
    double timeStep) const {
    requireValidStep(timeStep);

    std::vector<VectorND> velocities = velocitiesOf(bodies);
    std::vector<VectorND> accelerations = accelerationsOf(bodies, gravity);
    return makeStage(bodies, velocities, accelerations, timeStep);
}
