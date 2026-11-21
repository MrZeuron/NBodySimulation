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
    VectorND position3 = VectorND::zero3();
    VectorND secondPosition3 = VectorND::zero3();
    secondPosition3[0] = 2.0;
    VectorND velocity3 = VectorND::zero3();

    std::vector<CelestialBody> bodies3;
    bodies3.push_back(CelestialBody(3.0, position3, velocity3));
    bodies3.push_back(CelestialBody(2.0, secondPosition3, velocity3));
    Gravity3D gravity3(1.0);

    const GravityModel& model3 = gravity3;
    std::vector<VectorND> accelerations3 = model3.calculateAccelerations(bodies3);
    require(std::abs(accelerations3[0][0] - 0.5) < 1e-12,
            "3D acceleration of the first body is incorrect");
    require(std::abs(accelerations3[1][0] + 0.75) < 1e-12,
            "3D acceleration of the second body is incorrect");
    require(std::abs(model3.potentialEnergy(bodies3) + 3.0) < 1e-12,
            "3D potential energy is incorrect");

    std::vector<CelestialBody> reversedBodies3;
    reversedBodies3.push_back(bodies3[1]);
    reversedBodies3.push_back(bodies3[0]);
    std::vector<VectorND> reversedAccelerations3 =
        gravity3.calculateAccelerations(reversedBodies3);
    require(std::abs(reversedAccelerations3[0][0] - accelerations3[1][0]) < 1e-12,
            "body order changed the first 3D acceleration");
    require(std::abs(reversedAccelerations3[1][0] - accelerations3[0][0]) < 1e-12,
            "body order changed the second 3D acceleration");

    VectorND position4 = VectorND::zero4();
    VectorND secondPosition4 = VectorND::zero4();
    secondPosition4[3] = 2.0;
    VectorND velocity4 = VectorND::zero4();

    std::vector<CelestialBody> bodies4;
    bodies4.push_back(CelestialBody(3.0, position4, velocity4));
    bodies4.push_back(CelestialBody(2.0, secondPosition4, velocity4));
    Gravity4D gravity4(1.0);

    std::vector<VectorND> accelerations4 = gravity4.calculateAccelerations(bodies4);
    require(std::abs(accelerations4[0][3] - 0.25) < 1e-12,
            "4D acceleration of the first body is incorrect");
    require(std::abs(accelerations4[1][3] + 0.375) < 1e-12,
            "4D acceleration of the second body is incorrect");
    require(std::abs(gravity4.potentialEnergy(bodies4) + 0.75) < 1e-12,
            "4D potential energy is incorrect");

    bool dimensionRejected = false;
    try {
        gravity4.calculateAccelerations(bodies3);
    } catch (const std::invalid_argument&) {
        dimensionRejected = true;
    }
    require(dimensionRejected, "gravity model must reject the wrong dimension");

    std::vector<CelestialBody> samePosition;
    samePosition.push_back(CelestialBody(1.0, position3, velocity3));
    samePosition.push_back(CelestialBody(1.0, position3, velocity3));
    bool coincidentRejected = false;
    try {
        gravity3.calculateAccelerations(samePosition);
    } catch (const std::domain_error&) {
        coincidentRejected = true;
    }
    require(coincidentRejected, "coincident bodies must be rejected");

    std::cout << "GravityModel tests passed\n";
    return 0;
}
