#include "src/app/AppController.h"
#include "src/domain/MainSequenceStar.h"
#include "src/domain/RedGiant.h"
#include "src/domain/RockyPlanet.h"
#include "src/domain/GasGiant.h"
#include "src/domain/Moon.h"
#include "src/domain/Asteroid.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

VectorND makeVector(std::size_t dimension, double x, double y, double z, double w = 0.0);

int main(int argc, char* argv[]) {
    try {
        bool demo = false;
        std::size_t dimension = 3;
        int frames = 0;
        for (int i = 1; i < argc; ++i) {
            std::string argument = argv[i];
            if (argument == "--demo") {
                demo = true;
            } else if (argument == "--4d") {
                dimension = 4;
            } else if (argument == "--frames" && i + 1 < argc) {
                std::string number = argv[++i];
                std::size_t end = 0;
                frames = std::stoi(number, &end);
                if (end != number.size() || frames <= 0) {
                    throw std::invalid_argument("Frame count must be a positive integer");
                }
            } else {
                throw std::invalid_argument("Usage: simulator [--demo] [--4d] [--frames N]");
            }
        }

        // Место 1: масса, положение X/Y/Z[/W], начальная скорость.
        MainSequenceStar star(100.0, makeVector(dimension, 0, 0, 0),
                              makeVector(dimension, 0, -0.02975, 0));
        star.setName("Sun");

        // Место 2: наклонная орбита. Визуальный радиус не меняет гравитацию.
        RockyPlanet planet(1.0, makeVector(dimension, 10, 0, 5, 2),
                           makeVector(dimension, 0, 2.975, 0));
        planet.setName("Terra");
        std::vector<const CelestialBody*> bodies = {&star, &planet};

        // Готовый показ всех шести видов: запуск с аргументом --demo.
        RedGiant giant(0.5, makeVector(dimension, -12, -3, -4, -3),
                       makeVector(dimension, 0, -1.8, 0));
        GasGiant gas(0.1, makeVector(dimension, 0, 12, 2, 3),
                     makeVector(dimension, -2.5, 0, 0));
        Moon moon(0.001, makeVector(dimension, 12, 2, 5, -1),
                  makeVector(dimension, 0, 3.1, 0));
        Asteroid asteroid(0.001, makeVector(dimension, -5, -8, -2, 4),
                          makeVector(dimension, 1.5, -1, 0));
        if (demo) {
            bodies.push_back(&giant);
            bodies.push_back(&gas);
            bodies.push_back(&moon);
            bodies.push_back(&asteroid);
        }

        // Контроллер копирует тела с сохранением их типов.
        AppController app(bodies);
        app.run(frames);
    } catch (const std::exception& error) {
        std::cerr << "\x1b[0m\nSimulation stopped: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

VectorND makeVector(std::size_t dimension, double x, double y, double z, double w) {
    VectorND result(dimension);
    result[0] = x;
    result[1] = y;
    result[2] = z;
    if (dimension == 4) result[3] = w;
    return result;
}
