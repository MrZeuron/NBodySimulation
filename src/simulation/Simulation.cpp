#include "Simulation.h"

#include <cmath>
#include <stdexcept>

Simulation::Simulation(const std::vector<CelestialBody>& initialBodies,
                       const GravityModel& gravity,
                       const Integrator& integrator,
                       double timeStep)
    : initialBodies_(initialBodies),
      bodies_(initialBodies),
      gravity_(gravity),
      integrator_(&integrator),
      timeStep_(timeStep),
      time_(0.0),
      initialEnergy_(0.0) {
    requireValidStep(timeStep);
    requireValidBodies(initialBodies_);
    initialEnergy_ = energyOf(initialBodies_);
}

const std::vector<CelestialBody>& Simulation::bodies() const {
    return bodies_;
}

double Simulation::time() const {
    return time_;
}

double Simulation::timeStep() const {
    return timeStep_;
}

double Simulation::initialEnergy() const {
    return initialEnergy_;
}

double Simulation::totalEnergy() const {
    return energyOf(bodies_);
}

void Simulation::setTimeStep(double timeStep) {
    requireValidStep(timeStep);
    timeStep_ = timeStep;
}

void Simulation::setIntegrator(const Integrator& integrator) {
    integrator_ = &integrator;
}

void Simulation::step() {
    std::vector<CelestialBody> updated =
        integrator_->step(bodies_, gravity_, timeStep_);
    if (updated.size() != bodies_.size()) {
        throw std::invalid_argument("Integrator changed body count");
    }
    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        if (updated[i].mass() != bodies_[i].mass()) {
            throw std::invalid_argument("Integrator changed body mass");
        }
    }
    requireValidBodies(updated);

    double nextTime = time_ + timeStep_;
    if (!std::isfinite(nextTime) || nextTime <= time_) {
        throw std::overflow_error("Simulation time cannot advance");
    }

    bodies_.swap(updated);
    time_ = nextTime;
}

void Simulation::reset() {
    bodies_ = initialBodies_;
    time_ = 0.0;
}

void Simulation::requireValidStep(double timeStep) const {
    if (!std::isfinite(timeStep) || timeStep <= 0.0) {
        throw std::invalid_argument("Time step must be finite and positive");
    }
}

void Simulation::requireValidBodies(
    const std::vector<CelestialBody>& bodies) const {
    if (bodies.size() < 2) {
        throw std::invalid_argument("Simulation needs at least two bodies");
    }

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        if (bodies[i].position().dimension() != gravity_.dimension()) {
            throw std::invalid_argument("Body dimension does not match gravity model");
        }
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            VectorND difference = bodies[j].position() - bodies[i].position();
            double distanceSquared = difference.squaredNorm();
            if (!std::isfinite(distanceSquared) || distanceSquared <= 0.0) {
                throw std::domain_error("Body positions must be distinct and finite");
            }
        }
    }
}

double Simulation::energyOf(const std::vector<CelestialBody>& bodies) const {
    double kineticEnergy = 0.0;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        kineticEnergy += bodies[i].mass() * bodies[i].velocity().squaredNorm() / 2.0;
        if (!std::isfinite(kineticEnergy)) {
            throw std::overflow_error("Kinetic energy is not finite");
        }
    }

    double energy = kineticEnergy + gravity_.potentialEnergy(bodies);
    if (!std::isfinite(energy)) {
        throw std::overflow_error("Total energy is not finite");
    }
    return energy;
}
