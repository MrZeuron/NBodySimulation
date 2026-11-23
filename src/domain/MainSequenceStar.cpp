#include "MainSequenceStar.h"

MainSequenceStar::MainSequenceStar(double mass, const VectorND& position, const VectorND& velocity)
    : Star(mass, position, velocity, 1.2, 1.0, 5800.0) {
}

Appearance MainSequenceStar::appearance() const {
    Appearance result = Star::appearance();
    result.symbol = 'S';
    return result;
}

const char* MainSequenceStar::typeName() const {
    return "MainSequenceStar";
}

std::unique_ptr<CelestialBody> MainSequenceStar::clone() const {
    return std::make_unique<MainSequenceStar>(*this);
}
