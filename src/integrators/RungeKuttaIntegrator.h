#pragma once

#include "Integrator.h"

// Общие операции для методов Эйлера и Рунге — Кутты.
class RungeKuttaIntegrator : public Integrator {
protected:
    std::vector<VectorND> velocitiesOf(
        const std::vector<CelestialBody>& bodies) const;

    std::vector<CelestialBody> makeStage(
        const std::vector<CelestialBody>& bodies,
        const std::vector<VectorND>& positionSlopes,
        const std::vector<VectorND>& velocitySlopes,
        double stageStep) const;
};
