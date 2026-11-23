#include "../src/app/AppController.h"
#include "../src/domain/MainSequenceStar.h"
#include "../src/domain/RedGiant.h"
#include "../src/domain/RockyPlanet.h"
#include "../src/domain/GasGiant.h"
#include "../src/domain/Moon.h"
#include "../src/domain/Asteroid.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void checkHierarchy(std::size_t dimension) {
    VectorND origin(dimension);
    VectorND position(dimension);
    VectorND velocity(dimension);
    position[0] = 10.0;
    if (dimension == 4) position[3] = 3.0;
    velocity[1] = 1.0;
    MainSequenceStar star(100, origin, VectorND(dimension));
    RedGiant red(100, origin, VectorND(dimension));
    RockyPlanet rocky(1, position, velocity);
    GasGiant gas(1, position, velocity);
    Moon moon(1, position, velocity);
    Asteroid asteroid(1, position, velocity);
    Star baseStar(1, position, velocity);
    Planet basePlanet(1, position, velocity);
    const CelestialBody* types[] = {&star, &red, &rocky, &gas, &moon, &asteroid,
                                    &baseStar, &basePlanet};
    Gravity3D gravity3(1);
    Gravity4D gravity4(1);
    const GravityModel* gravity = &gravity3;
    if (dimension == 4) gravity = &gravity4;
    EulerIntegrator euler;
    RK4Integrator rk4;
    VerletIntegrator verlet;
    const Integrator* methods[] = {&euler, &rk4, &verlet};

    for (const CelestialBody* type : types) {
        std::unique_ptr<CelestialBody> copy = type->clone();
        require(std::string(copy->typeName()) == type->typeName(), "clone lost dynamic type");
        require(copy->appearance().symbol == type->appearance().symbol, "clone lost appearance");
    }
    require(red.appearance().radius > star.appearance().radius, "red giant must look larger");
    require(gas.appearance().radius > rocky.appearance().radius, "gas giant must look larger");
    require(gas.appearance().surface == SurfaceStyle::Banded, "gas giant lost bands");
    require(moon.appearance().surface == SurfaceStyle::Cratered, "moon lost craters");
    require(asteroid.appearance().surface == SurfaceStyle::Irregular, "asteroid lost irregular outline");
    const char symbols[] = {'S', 'R', 'P', 'G', 'm', 'a'};
    for (int i = 0; i < 6; ++i) {
        require(types[i]->appearance().symbol == symbols[i], "types must have distinct symbols");
    }

    std::vector<const CelestialBody*> input = {&star, &gas};
    Simulation simulation(input, *gravity, euler, 0.01);
    Renderer renderer;
    renderer.record(simulation);
    std::string firstFrame = renderer.frame(simulation);
    require(firstFrame.find("MainSequenceStar") != std::string::npos, "star label missing");
    require(firstFrame.find("GasGiant") != std::string::npos, "planet label missing");
    for (const Integrator* method : methods) {
        simulation.setIntegrator(*method);
        simulation.step();
        require(std::string(simulation.body(1).typeName()) == "GasGiant", "step sliced body");
        require(simulation.body(1).position()[1] > 0, "body did not move");
        renderer.record(simulation);
    }
    require(simulation.body(1).position()[1] == simulation.bodies()[1].position()[1],
            "physical snapshot and polymorphic body differ");
    double energy = simulation.totalEnergy();
    double coordinate = simulation.body(1).position()[0];
    renderer.rotate4D(0.5);
    renderer.setScale(1.2);
    require(!renderer.frame(simulation).empty(), "empty scene");
    require(simulation.totalEnergy() == energy && simulation.body(1).position()[0] == coordinate,
            "view changed physics");
    simulation.reset();
    require(simulation.time() == 0 && simulation.body(1).position()[1] == 0, "reset failed");
    require(std::string(simulation.body(1).typeName()) == "GasGiant", "reset sliced body");
    // Внешние исходные объекты не принадлежат Simulation и не изменяются.
    require(gas.position()[1] == 0, "simulation modified source object");

    std::vector<CelestialBody> first = {star, rocky};
    std::vector<CelestialBody> second = {red, gas};
    std::vector<VectorND> acceleration1 = gravity->calculateAccelerations(first);
    std::vector<VectorND> acceleration2 = gravity->calculateAccelerations(second);
    require(acceleration1[0][0] == acceleration2[0][0], "appearance changed gravity");

    Gravity3D stronger3(2);
    Gravity4D stronger4(2);
    if (dimension == 3) simulation.setGravityModel(stronger3);
    else simulation.setGravityModel(stronger4);
    require(simulation.initialEnergy() != energy, "model switch did not update energy");
}

int main() {
    checkHierarchy(3);
    checkHierarchy(4);
    VectorND position(4);
    position[0] = 2;
    position[3] = 4;
    Projection4D projection(0);
    VectorND before = projection.project(position);
    projection.setAngle(0.7);
    require(projection.project(position)[0] != before[0], "4D rotation has no effect");
    require(position[0] == 2 && position[3] == 4, "projection changed source coordinates");

    std::unique_ptr<Simulation> independent;
    Gravity3D lifetimeGravity(1);
    VerletIntegrator lifetimeMethod;
    {
        VectorND otherPosition(3);
        otherPosition[0] = 10;
        RedGiant temporaryStar(100, VectorND(3), VectorND(3));
        Moon temporaryMoon(1, otherPosition, VectorND(3));
        std::vector<const CelestialBody*> temporary = {&temporaryStar, &temporaryMoon};
        independent = std::make_unique<Simulation>(temporary, lifetimeGravity, lifetimeMethod, 0.01);
    }
    independent->step();
    require(std::string(independent->body(0).typeName()) == "RedGiant",
            "simulation did not retain its own body copies");
    bool nullRejected = false;
    try {
        std::vector<const CelestialBody*> invalid = {nullptr, nullptr};
        Simulation broken(invalid, lifetimeGravity, lifetimeMethod, 0.01);
    } catch (const std::invalid_argument&) {
        nullRejected = true;
    }
    require(nullRejected, "null bodies must be rejected");

    VectorND p(3);
    p[0] = 10;
    MainSequenceStar star(100, VectorND(3), VectorND(3));
    RockyPlanet planet(1, p, VectorND(3));
    std::vector<const CelestialBody*> input = {&star, &planet};
    AppController app(input);
    app.handleKey('2');
    app.handleKey(' ');
    app.run(1);
    require(app.simulation().time() == 0, "pause failed");
    app.handleKey(' ');
    app.run(1);
    require(app.simulation().time() > 0, "resume failed");
    app.handleKey('r');
    require(app.simulation().time() == 0, "controller reset failed");
    app.handleKey('d');
    require(app.simulation().dimension() == 4, "4D switch failed");
    app.handleKey('1'); app.run(1);
    app.handleKey('3'); app.run(1);
    app.handleKey('d');
    require(app.simulation().dimension() == 3, "3D switch failed");
    require(std::string(app.simulation().body(0).typeName()) == "MainSequenceStar",
            "dimension switch sliced star");

    std::cout << "Architecture tests passed\n";
    return 0;
}
