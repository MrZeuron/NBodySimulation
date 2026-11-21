#pragma once

#include "RungeKuttaIntegrator.h"

class RK4Integrator : public RungeKuttaIntegrator {
public:
    std::vector<CelestialBody> step(
        const std::vector<CelestialBody>& bodies,
        const GravityModel& gravity,
        double timeStep) const override;
};
