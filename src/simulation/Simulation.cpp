#include "Simulation.h"

#include <cmath>
#include <stdexcept>
#include <utility>

Simulation::Simulation(const std::vector<CelestialBody>& initialBodies,
                       const GravityModel& gravity, const Integrator& integrator,
                       double timeStep)
    : gravity_(&gravity), integrator_(&integrator), timeStep_(timeStep),
      time_(0.0), initialEnergy_(0.0) {
    requireValidStep(timeStep);
    requireValidBodies(initialBodies);
    for (const CelestialBody& body : initialBodies) {
        initialBodies_.push_back(body.clone());
        bodies_.push_back(body.clone());
    }
    initialEnergy_ = energyOf(initialBodies);
}

Simulation::Simulation(const std::vector<const CelestialBody*>& initialBodies,
                       const GravityModel& gravity, const Integrator& integrator,
                       double timeStep)
    : gravity_(&gravity), integrator_(&integrator), timeStep_(timeStep),
      time_(0.0), initialEnergy_(0.0) {
    requireValidStep(timeStep);
    for (const CelestialBody* body : initialBodies) {
        if (body == nullptr) {
            throw std::invalid_argument("Null celestial body");
        }
        initialBodies_.push_back(body->clone());
        bodies_.push_back(body->clone());
    }
    std::vector<CelestialBody> snapshot = bodies();
    requireValidBodies(snapshot);
    initialEnergy_ = energyOf(snapshot);
}

std::vector<CelestialBody> Simulation::bodies() const {
    std::vector<CelestialBody> snapshot;
    for (const std::unique_ptr<CelestialBody>& body : bodies_) {
        snapshot.push_back(*body);
    }
    return snapshot;
}

const CelestialBody& Simulation::body(std::size_t index) const {
    return *bodies_.at(index);
}

std::size_t Simulation::bodyCount() const {
    return bodies_.size();
}

std::size_t Simulation::dimension() const {
    return gravity_->dimension();
}

void Simulation::setGravityModel(const GravityModel& gravity) {
    if (gravity.dimension() != dimension()) {
        throw std::invalid_argument("Changing dimension requires a new initial scenario");
    }
    // Проверяем новую модель до изменения текущей симуляции.
    gravity.potentialEnergy(bodies());
    std::vector<CelestialBody> initial;
    for (const std::unique_ptr<CelestialBody>& body : initialBodies_) {
        initial.push_back(*body);
    }
    double kinetic = 0.0;
    for (const CelestialBody& body : initial) {
        kinetic += body.mass() * body.velocity().squaredNorm() / 2.0;
    }
    double energy = kinetic + gravity.potentialEnergy(initial);
    if (!std::isfinite(energy)) {
        throw std::overflow_error("Initial energy is not finite");
    }
    gravity_ = &gravity;
    initialEnergy_ = energy;
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
    return energyOf(bodies());
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
        integrator_->step(bodies(), *gravity_, timeStep_);
    if (updated.size() != bodies_.size()) {
        throw std::invalid_argument("Integrator changed body count");
    }
    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        if (updated[i].mass() != bodies_[i]->mass()) {
            throw std::invalid_argument("Integrator changed body mass");
        }
    }
    requireValidBodies(updated);

    double nextTime = time_ + timeStep_;
    if (!std::isfinite(nextTime) || nextTime <= time_) {
        throw std::overflow_error("Simulation time cannot advance");
    }

    // Готовим новый набор целиком: при ошибке старое состояние сохраняется.
    std::vector<std::unique_ptr<CelestialBody>> nextBodies;
    for (std::size_t i = 0; i < updated.size(); ++i) {
        std::unique_ptr<CelestialBody> copy = bodies_[i]->clone();
        copy->setPosition(updated[i].position());
        copy->setVelocity(updated[i].velocity());
        nextBodies.push_back(std::move(copy));
    }
    bodies_.swap(nextBodies);
    time_ = nextTime;
}

void Simulation::reset() {
    std::vector<std::unique_ptr<CelestialBody>> restored;
    for (const std::unique_ptr<CelestialBody>& body : initialBodies_) {
        restored.push_back(body->clone());
    }
    bodies_.swap(restored);
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
        if (bodies[i].position().dimension() != gravity_->dimension()) {
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

    double energy = kineticEnergy + gravity_->potentialEnergy(bodies);
    if (!std::isfinite(energy)) {
        throw std::overflow_error("Total energy is not finite");
    }
    return energy;
}
