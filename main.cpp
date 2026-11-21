#include "src/domain/CelestialBody.h"
#include "src/integrators/VerletIntegrator.h"
#include "src/physics/Gravity3D.h"
#include "src/simulation/Simulation.h"

#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <vector>

const int screenWidth = 61;
const int screenHeight = 17;

VectorND makeVector3(double x, double y, double z);
bool project(double x, double y, double z, int& column, int& row);
void draw(const Simulation& simulation,
          const std::vector<std::vector<VectorND>>& trails);

int main() {
    try {
        std::vector<CelestialBody> bodies;

        // Место 1: тяжёлое тело около центра. Меняй массу, координаты и скорость.
        bodies.push_back(CelestialBody(
            100.0, makeVector3(0.0, 0.0, 0.0),
            makeVector3(0.0, -0.02975, 0.0)));

        // Место 2: орбита наклонена, поэтому меняется и координата Z.
        bodies.push_back(CelestialBody(
            1.0, makeVector3(10.0, 0.0, 5.0),
            makeVector3(0.0, 2.975, 0.0)));

        // Новое тело можно добавить такой же строкой перед созданием Simulation:
        // bodies.push_back(CelestialBody(
        //     1.0, makeVector3(-12.0, 0.0, 0.0),
        //     makeVector3(0.0, -2.9, 0.0)));

        Gravity3D gravity(1.0);
        VerletIntegrator integrator;
        Simulation simulation(bodies, gravity, integrator, 0.02);
        std::vector<std::vector<VectorND>> trails(bodies.size());

        std::cout << "\x1b[2J";
        while (true) {
            draw(simulation, trails);
            for (int i = 0; i < 5; ++i) {
                simulation.step();
            }
            for (std::size_t i = 0; i < trails.size(); ++i) {
                trails[i].push_back(simulation.bodies()[i].position());
                if (trails[i].size() > 100) {
                    trails[i].erase(trails[i].begin());
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } catch (const std::exception& error) {
        std::cerr << "Simulation stopped: " << error.what() << '\n';
        return 1;
    }
}

VectorND makeVector3(double x, double y, double z) {
    VectorND result(3);
    result[0] = x;
    result[1] = y;
    result[2] = z;
    return result;
}

// Наклонный вид: X идёт вправо-вниз, Y влево-вниз, Z вверх.
bool project(double x, double y, double z, int& column, int& row) {
    double screenX = screenWidth / 2.0 + (x - y) * 0.9;
    double screenY = screenHeight / 2.0 + (x + y) * 0.45 - z * 0.9;
    if (!std::isfinite(screenX) || !std::isfinite(screenY)
        || screenX < 0.0 || screenX >= screenWidth
        || screenY < 0.0 || screenY >= screenHeight) {
        return false;
    }
    column = static_cast<int>(screenX);
    row = static_cast<int>(screenY);
    return true;
}

void draw(const Simulation& simulation,
          const std::vector<std::vector<VectorND>>& trails) {
    std::string screen[screenHeight];
    double bodyDepth[screenHeight][screenWidth];
    for (int row = 0; row < screenHeight; ++row) {
        screen[row] = std::string(screenWidth, ' ');
        for (int column = 0; column < screenWidth; ++column) {
            bodyDepth[row][column] = -std::numeric_limits<double>::infinity();
        }
    }

    // Точки на плоскости Z = 0 помогают увидеть объём сцены.
    for (int x = -20; x <= 20; x += 5) {
        for (int y = -20; y <= 20; y += 5) {
            int column, row;
            if (project(x, y, 0.0, column, row)) {
                screen[row][column] = '.';
            }
        }
    }

    for (int distance = -14; distance <= 14; ++distance) {
        int column, row;
        if (project(distance, 0.0, 0.0, column, row)) {
            screen[row][column] = '\\';
        }
        if (project(0.0, distance, 0.0, column, row)) {
            screen[row][column] = '/';
        }
    }
    for (int z = -7; z <= 7; ++z) {
        int column, row;
        if (project(0.0, 0.0, z, column, row)) {
            screen[row][column] = '|';
        }
    }

    int column, row;
    if (project(0.0, 0.0, 0.0, column, row)) screen[row][column] = '+';
    if (project(16.0, 0.0, 0.0, column, row)) screen[row][column] = 'X';
    if (project(0.0, 16.0, 0.0, column, row)) screen[row][column] = 'Y';
    if (project(0.0, 0.0, 8.0, column, row)) screen[row][column] = 'Z';

    for (std::size_t i = 0; i < trails.size(); ++i) {
        for (std::size_t j = 0; j < trails[i].size(); ++j) {
            const VectorND& position = trails[i][j];
            if (project(position[0], position[1], position[2], column, row)) {
                screen[row][column] = ':';
            }
        }
    }

    const std::vector<CelestialBody>& bodies = simulation.bodies();
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        const VectorND& position = bodies[i].position();
        if (!project(position[0], position[1], position[2], column, row)) {
            continue; // Тело вне экрана всё равно остаётся в симуляции.
        }
        double depth = position[0] + position[1] + position[2];
        if (depth > bodyDepth[row][column]) {
            char symbol = i < 9 ? static_cast<char>('1' + i) : '*';
            screen[row][column] = symbol;
            bodyDepth[row][column] = depth;
        }
    }

    std::string frame = "\x1b[H";
    frame += "3D ASCII view: X/Y floor, Z vertical\n";
    frame += "Time: " + std::to_string(simulation.time())
             + "    Bodies: " + std::to_string(bodies.size()) + "\n";
    frame += "+" + std::string(screenWidth, '-') + "+\n";
    for (int row = 0; row < screenHeight; ++row) {
        frame += "|" + screen[row] + "|\n";
    }
    frame += "+" + std::string(screenWidth, '-') + "+\n";
    frame += "1, 2, ... = bodies; : = trail; Ctrl+C to stop\n";
    frame += "\x1b[J";
    std::cout << frame << std::flush;
}
