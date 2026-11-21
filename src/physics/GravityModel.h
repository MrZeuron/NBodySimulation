#pragma once

#include "../domain/CelestialBody.h"

#include <vector>

// Общее правило расчёта гравитации для всей системы тел.
class GravityModel {
public:
    virtual ~GravityModel();

    virtual std::size_t dimension() const = 0;
    virtual std::vector<VectorND> calculateAccelerations(
        const std::vector<CelestialBody>& bodies) const = 0;
    virtual double potentialEnergy(const std::vector<CelestialBody>& bodies) const = 0;
};
