#include "Simulation.h"
#include "EulerIntegrator.h"
#include "RK4Integrator.h"
#include "VerletIntegrator.h"
#include "Gravity3D.h"
#include "Gravity4D.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

int main() {
    VectorND firstPosition = VectorND::zero3();
    VectorND secondPosition = VectorND::zero3();
    secondPosition[0] = 2.0;
    VectorND velocity = VectorND::zero3();

    std::vector<CelestialBody> bodies;
    bodies.push_back(CelestialBody(1.0, firstPosition, velocity));
    bodies.push_back(CelestialBody(1.0, secondPosition, velocity));

    Gravity3D gravity3(1.0);
    EulerIntegrator euler;
    VerletIntegrator verlet;
    Simulation simulation(bodies, gravity3, euler, 0.1);

    require(simulation.time() == 0.0, "initial simulation time is incorrect");
    require(std::abs(simulation.initialEnergy() + 0.5) < 1e-12,
            "initial energy is incorrect");
    require(std::abs(simulation.totalEnergy() + 0.5) < 1e-12,
            "current energy is incorrect");

    simulation.step();
    require(std::abs(simulation.time() - 0.1) < 1e-12,
            "simulation time was not advanced");
    require(std::abs(simulation.bodies()[0].velocity()[0] - 0.025) < 1e-12,
            "Euler step did not update the first velocity");
    require(bodies[0].velocity()[0] == 0.0,
            "simulation changed the input bodies");

    simulation.reset();
    require(simulation.time() == 0.0, "reset did not clear time");
    require(simulation.bodies()[0].velocity()[0] == 0.0,
            "reset did not restore the initial bodies");
    require(std::abs(simulation.totalEnergy() - simulation.initialEnergy()) < 1e-12,
            "reset did not restore the initial energy");

    simulation.setIntegrator(verlet);
    simulation.step();
    require(simulation.bodies()[0].position()[0] > 0.0,
            "Verlet step did not update position");
    simulation.reset();
    simulation.setTimeStep(0.05);
    simulation.step();
    require(std::abs(simulation.time() - 0.05) < 1e-12,
            "new time step was not used");

    bool invalidStepRejected = false;
    try {
        simulation.setTimeStep(0.0);
    } catch (const std::invalid_argument&) {
        invalidStepRejected = true;
    }
    require(invalidStepRejected && simulation.timeStep() == 0.05,
            "invalid time step changed the simulation");

    VectorND firstPosition4 = VectorND::zero4();
    VectorND secondPosition4 = VectorND::zero4();
    secondPosition4[3] = 2.0;
    std::vector<CelestialBody> bodies4;
    bodies4.push_back(CelestialBody(1.0, firstPosition4, VectorND::zero4()));
    bodies4.push_back(CelestialBody(1.0, secondPosition4, VectorND::zero4()));
    Gravity4D gravity4(1.0);
    RK4Integrator rk4;
    Simulation simulation4(bodies4, gravity4, rk4, 0.1);
    require(std::abs(simulation4.initialEnergy() + 0.125) < 1e-12,
            "4D initial energy is incorrect");
    simulation4.step();
    require(simulation4.bodies()[0].velocity()[3] > 0.0,
            "4D RK4 step did not move bodies toward each other");

    bool coincidentRejected = false;
    try {
        std::vector<CelestialBody> samePosition;
        samePosition.push_back(CelestialBody(1.0, firstPosition, velocity));
        samePosition.push_back(CelestialBody(1.0, firstPosition, velocity));
        Simulation invalid(samePosition, gravity3, euler, 0.1);
    } catch (const std::domain_error&) {
        coincidentRejected = true;
    }
    require(coincidentRejected, "coincident initial positions must be rejected");

    VectorND rightVelocity = VectorND::zero3();
    VectorND leftVelocity = VectorND::zero3();
    rightVelocity[0] = 1.0;
    leftVelocity[0] = -1.0;
    std::vector<CelestialBody> approaching;
    approaching.push_back(CelestialBody(1.0, firstPosition, rightVelocity));
    approaching.push_back(CelestialBody(1.0, secondPosition, leftVelocity));
    Simulation collision(approaching, gravity3, euler, 1.0);

    bool collisionRejected = false;
    try {
        collision.step();
    } catch (const std::domain_error&) {
        collisionRejected = true;
    }
    require(collisionRejected && collision.time() == 0.0,
            "failed step changed simulation time");
    require(collision.bodies()[0].position()[0] == 0.0,
            "failed step changed body positions");

    std::cout << "Simulation tests passed\n";
    return 0;
}
