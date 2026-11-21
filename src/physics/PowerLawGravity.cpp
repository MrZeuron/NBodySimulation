#include "PowerLawGravity.h"

#include <cmath>
#include <stdexcept>

PowerLawGravity::PowerLawGravity(double gravitationalConstant)
    : gravitationalConstant_(gravitationalConstant) {
    if (!std::isfinite(gravitationalConstant) || gravitationalConstant <= 0.0) {
        throw std::invalid_argument("Gravitational constant must be finite and positive");
    }
}

std::vector<VectorND> PowerLawGravity::calculateAccelerations(
    const std::vector<CelestialBody>& bodies) const {
    requireValidBodies(bodies);
    std::vector<VectorND> accelerations(bodies.size(), VectorND(dimension()));

    // Каждая пара даёт вклад в ускорение обоих тел.
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            VectorND difference = bodies[j].position() - bodies[i].position();
            double distanceSquared = requireSquaredDistance(difference);

            double denominator = distanceSquared * distanceSquared;
            if (dimension() == 3) {
                denominator = distanceSquared * std::sqrt(distanceSquared);
            }
            if (!std::isfinite(denominator) || denominator <= 0.0) {
                throw std::overflow_error("Distance is too small for gravity calculation");
            }

            double factor = gravitationalConstant_ / denominator;
            if (!std::isfinite(factor)) {
                throw std::overflow_error("Acceleration is too large");
            }

            accelerations[i] += difference * (factor * bodies[j].mass());
            accelerations[j] -= difference * (factor * bodies[i].mass());
        }
    }

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t axis = 0; axis < dimension(); ++axis) {
            if (!std::isfinite(accelerations[i][axis])) {
                throw std::overflow_error("Acceleration is not finite");
            }
        }
    }
    return accelerations;
}

double PowerLawGravity::potentialEnergy(
    const std::vector<CelestialBody>& bodies) const {
    requireValidBodies(bodies);

    double energy = 0.0;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            VectorND difference = bodies[j].position() - bodies[i].position();
            double distanceSquared = requireSquaredDistance(difference);

            double denominator = 2.0 * distanceSquared;
            if (dimension() == 3) {
                denominator = std::sqrt(distanceSquared);
            }
            if (!std::isfinite(denominator) || denominator <= 0.0) {
                throw std::overflow_error("Distance is too small for energy calculation");
            }

            double pairEnergy = -gravitationalConstant_ * bodies[i].mass()
                                * bodies[j].mass() / denominator;
            energy += pairEnergy;
            if (!std::isfinite(energy)) {
                throw std::overflow_error("Potential energy is not finite");
            }
        }
    }
    return energy;
}

void PowerLawGravity::requireValidBodies(
    const std::vector<CelestialBody>& bodies) const {
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        if (bodies[i].position().dimension() != dimension()) {
            throw std::invalid_argument("Body dimension does not match gravity model");
        }
    }
}

double PowerLawGravity::requireSquaredDistance(const VectorND& difference) const {
    double distanceSquared = difference.squaredNorm();
    if (!std::isfinite(distanceSquared) || distanceSquared <= 0.0) {
        throw std::domain_error("Bodies are too close or distance is not finite");
    }
    return distanceSquared;
}
