#pragma once

#include "../integrators/Integrator.h"

// Хранит состояние системы и управляет шагом расчёта.
// Модель и интегратор должны существовать дольше объекта Simulation.
class Simulation {
public:
    Simulation(const std::vector<CelestialBody>& initialBodies,
               const GravityModel& gravity,
               const Integrator& integrator,
               double timeStep);

    Simulation(const std::vector<const CelestialBody*>& initialBodies,
               const GravityModel& gravity, const Integrator& integrator,
               double timeStep);

    // Численный снимок содержит только общую физическую часть.
    std::vector<CelestialBody> bodies() const;
    const CelestialBody& body(std::size_t index) const;
    std::size_t bodyCount() const;
    std::size_t dimension() const;
    void setGravityModel(const GravityModel& gravity);
    double time() const;
    double timeStep() const;
    double initialEnergy() const;
    double totalEnergy() const;

    void setTimeStep(double timeStep);
    void setIntegrator(const Integrator& integrator);
    void step();
    void reset();

private:
    std::vector<std::unique_ptr<CelestialBody>> initialBodies_;
    std::vector<std::unique_ptr<CelestialBody>> bodies_;
    const GravityModel* gravity_;
    const Integrator* integrator_;
    double timeStep_;
    double time_;
    double initialEnergy_;

    void requireValidStep(double timeStep) const;
    void requireValidBodies(const std::vector<CelestialBody>& bodies) const;
    double energyOf(const std::vector<CelestialBody>& bodies) const;
};
