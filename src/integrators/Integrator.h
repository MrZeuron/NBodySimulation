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
    void requireValidStep(double timeStep) const;
    std::vector<VectorND> accelerationsOf(
        const std::vector<CelestialBody>& bodies,
        const GravityModel& gravity) const;
};
