#pragma once

#include "../domain/CelestialBody.h"

#include <vector>

// Общее правило расчёта гравитации для всей системы тел.
class GravityModel {
public:
    virtual ~GravityModel();

    virtual std::size_t dimension() const = 0;
    virtual std::vector<VectorND> calculateAccelerations(
        const std::vector<CelestialBody>& bodies) const;
    virtual double potentialEnergy(const std::vector<CelestialBody>& bodies) const;

protected:
    explicit GravityModel(double gravitationalConstant = 1.0);

private:
    double gravitationalConstant_;
    void requireValidBodies(const std::vector<CelestialBody>& bodies) const;
    double requireSquaredDistance(const VectorND& difference) const;
};
