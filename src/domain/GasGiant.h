#pragma once
#include "Planet.h"

class GasGiant : public Planet {
public:
    GasGiant(double mass, const VectorND& position, const VectorND& velocity);
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
};
