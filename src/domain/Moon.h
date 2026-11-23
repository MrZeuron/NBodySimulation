#pragma once
#include "CelestialBody.h"

class Moon : public CelestialBody {
public:
    Moon(double mass, const VectorND& position, const VectorND& velocity);
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
};
