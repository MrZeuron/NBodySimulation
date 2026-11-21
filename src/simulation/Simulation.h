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

    const std::vector<CelestialBody>& bodies() const;
    double time() const;
    double timeStep() const;
    double initialEnergy() const;
    double totalEnergy() const;

    void setTimeStep(double timeStep);
    void setIntegrator(const Integrator& integrator);
    void step();
    void reset();

private:
    std::vector<CelestialBody> initialBodies_;
    std::vector<CelestialBody> bodies_;
    const GravityModel& gravity_;
    const Integrator* integrator_;
    double timeStep_;
    double time_;
    double initialEnergy_;

    void requireValidStep(double timeStep) const;
    void requireValidBodies(const std::vector<CelestialBody>& bodies) const;
    double energyOf(const std::vector<CelestialBody>& bodies) const;
};
