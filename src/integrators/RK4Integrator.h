#pragma once

#include "Integrator.h"

class RK4Integrator : public Integrator {
public:
    std::vector<CelestialBody> step(
        const std::vector<CelestialBody>& bodies,
        const GravityModel& gravity,
        double timeStep) const override;
};
