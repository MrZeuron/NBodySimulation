#pragma once

#include "GravityModel.h"

// Общий расчёт для закона притяжения в 3D и 4D.
class PowerLawGravity : public GravityModel {
public:
    std::vector<VectorND> calculateAccelerations(
        const std::vector<CelestialBody>& bodies) const override;
    double potentialEnergy(const std::vector<CelestialBody>& bodies) const override;

protected:
    explicit PowerLawGravity(double gravitationalConstant);

private:
    double gravitationalConstant_;

    void requireValidBodies(const std::vector<CelestialBody>& bodies) const;
    double requireSquaredDistance(const VectorND& difference) const;
};
