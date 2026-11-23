#pragma once
#include "CelestialBody.h"

class Planet : public CelestialBody {
public:
    Planet(double mass, const VectorND& position, const VectorND& velocity,
           double radius = 0.7, BodyColor color = {100, 170, 240});
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
private:
    double radius_;
    BodyColor color_;
};
