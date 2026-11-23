#pragma once
#include "Planet.h"

class RockyPlanet : public Planet {
public:
    RockyPlanet(double mass, const VectorND& position, const VectorND& velocity);
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
};
