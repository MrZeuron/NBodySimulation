#pragma once
#include "CelestialBody.h"

class Star : public CelestialBody {
public:
    Star(double mass, const VectorND& position, const VectorND& velocity,
         double radius = 1.2, double luminosity = 1.0, double temperature = 5800.0);
    double luminosity() const;
    double temperature() const;
    Appearance appearance() const override;
    const char* typeName() const override;
    std::unique_ptr<CelestialBody> clone() const override;
private:
    double radius_;
    double luminosity_;
    double temperature_;
};
