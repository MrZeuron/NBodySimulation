#pragma once
#include "Star.h"

class RedGiant : public Star {
public:
    RedGiant(double mass, const VectorND& position, const VectorND& velocity);
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
};
