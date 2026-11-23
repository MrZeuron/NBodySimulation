#pragma once

#include "../physics/GravityModel.h"

// Вычисляет новое состояние всех тел за один шаг времени.
class Integrator {
public:
    virtual ~Integrator();

    virtual std::vector<CelestialBody> step(
        const std::vector<CelestialBody>& bodies,
        const GravityModel& gravity,
        double timeStep) const = 0;

protected:
    std::vector<VectorND> velocitiesOf(
        const std::vector<CelestialBody>& bodies) const;
    std::vector<CelestialBody> makeStage(
        const std::vector<CelestialBody>& bodies,
        const std::vector<VectorND>& positionSlopes,
        const std::vector<VectorND>& velocitySlopes,
        double stageStep) const;
    void requireValidStep(double timeStep) const;
    std::vector<VectorND> accelerationsOf(
        const std::vector<CelestialBody>& bodies,
        const GravityModel& gravity) const;
};
