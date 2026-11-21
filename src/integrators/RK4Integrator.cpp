#include "RK4Integrator.h"

std::vector<CelestialBody> RK4Integrator::step(
    const std::vector<CelestialBody>& bodies,
    const GravityModel& gravity,
    double timeStep) const {
    requireValidStep(timeStep);

    std::vector<VectorND> velocity1 = velocitiesOf(bodies);
    std::vector<VectorND> acceleration1 = accelerationsOf(bodies, gravity);

    std::vector<CelestialBody> stage2 = makeStage(
        bodies, velocity1, acceleration1, timeStep / 2.0);
    std::vector<VectorND> velocity2 = velocitiesOf(stage2);
    std::vector<VectorND> acceleration2 = accelerationsOf(stage2, gravity);

    std::vector<CelestialBody> stage3 = makeStage(
        bodies, velocity2, acceleration2, timeStep / 2.0);
    std::vector<VectorND> velocity3 = velocitiesOf(stage3);
    std::vector<VectorND> acceleration3 = accelerationsOf(stage3, gravity);

    std::vector<CelestialBody> stage4 = makeStage(
        bodies, velocity3, acceleration3, timeStep);
    std::vector<VectorND> velocity4 = velocitiesOf(stage4);
    std::vector<VectorND> acceleration4 = accelerationsOf(stage4, gravity);

    std::vector<CelestialBody> result;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        VectorND positionChange = velocity1[i] + velocity2[i] * 2.0
                                  + velocity3[i] * 2.0 + velocity4[i];
        VectorND velocityChange = acceleration1[i] + acceleration2[i] * 2.0
                                  + acceleration3[i] * 2.0 + acceleration4[i];

        VectorND position = bodies[i].position() + positionChange * (timeStep / 6.0);
        VectorND velocity = bodies[i].velocity() + velocityChange * (timeStep / 6.0);
        result.push_back(CelestialBody(bodies[i].mass(), position, velocity));
    }
    return result;
}
