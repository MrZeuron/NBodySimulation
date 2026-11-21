#include "EulerIntegrator.h"
#include "Gravity3D.h"
#include "Gravity4D.h"
#include "RK4Integrator.h"
#include "VerletIntegrator.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

// Простая модель с постоянным ускорением проверяет формулы трёх методов.
class ConstantAcceleration : public GravityModel {
public:
    ConstantAcceleration(std::size_t dimension, double acceleration)
        : dimension_(dimension), acceleration_(acceleration) {
    }

    std::size_t dimension() const override {
        return dimension_;
    }

    std::vector<VectorND> calculateAccelerations(
        const std::vector<CelestialBody>& bodies) const override {
        std::vector<VectorND> result(bodies.size(), VectorND(dimension_));
        for (std::size_t i = 0; i < result.size(); ++i) {
            result[i][dimension_ - 1] = acceleration_;
        }
        return result;
    }

    double potentialEnergy(const std::vector<CelestialBody>&) const override {
        return 0.0;
    }

private:
    std::size_t dimension_;
    double acceleration_;
};

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void checkStep(const Integrator& integrator, std::size_t dimension,
               double expectedPosition) {
    VectorND position(dimension);
    VectorND velocity(dimension);
    velocity[dimension - 1] = 2.0;

    std::vector<CelestialBody> bodies;
    bodies.push_back(CelestialBody(1.0, position, velocity));
    ConstantAcceleration gravity(dimension, 3.0);

    std::vector<CelestialBody> result = integrator.step(bodies, gravity, 0.5);
    require(result.size() == 1, "integrator changed body count");
    require(std::abs(result[0].position()[dimension - 1] - expectedPosition) < 1e-12,
            "integrator position is incorrect");
    require(std::abs(result[0].velocity()[dimension - 1] - 3.5) < 1e-12,
            "integrator velocity is incorrect");
    require(bodies[0].position()[dimension - 1] == 0.0,
            "integrator changed the original state");
}

void checkGravityStep(const Integrator& integrator, const GravityModel& gravity) {
    std::size_t axis = gravity.dimension() - 1;
    VectorND firstPosition(gravity.dimension());
    VectorND secondPosition(gravity.dimension());
    secondPosition[axis] = 2.0;
    VectorND velocity(gravity.dimension());

    std::vector<CelestialBody> bodies;
    bodies.push_back(CelestialBody(1.0, firstPosition, velocity));
    bodies.push_back(CelestialBody(1.0, secondPosition, velocity));

    std::vector<CelestialBody> result = integrator.step(bodies, gravity, 0.1);
    require(result[0].velocity()[axis] > 0.0,
            "first body must move toward the second body");
    require(result[1].velocity()[axis] < 0.0,
            "second body must move toward the first body");
    require(bodies[0].position()[axis] == 0.0,
            "gravity step changed the original state");
}

int main() {
    EulerIntegrator euler;
    RK4Integrator rk4;
    VerletIntegrator verlet;

    checkStep(euler, 3, 1.0);
    checkStep(rk4, 3, 1.375);
    checkStep(verlet, 3, 1.375);

    checkStep(euler, 4, 1.0);
    checkStep(rk4, 4, 1.375);
    checkStep(verlet, 4, 1.375);

    Gravity3D gravity3(1.0);
    Gravity4D gravity4(1.0);
    checkGravityStep(euler, gravity3);
    checkGravityStep(rk4, gravity3);
    checkGravityStep(verlet, gravity3);
    checkGravityStep(euler, gravity4);
    checkGravityStep(rk4, gravity4);
    checkGravityStep(verlet, gravity4);

    std::vector<CelestialBody> emptyBodies;
    ConstantAcceleration gravity(3, 3.0);
    bool invalidStepRejected = false;
    try {
        euler.step(emptyBodies, gravity, 0.0);
    } catch (const std::invalid_argument&) {
        invalidStepRejected = true;
    }
    require(invalidStepRejected, "zero time step must be rejected");

    std::cout << "Integrator tests passed\n";
    return 0;
}
