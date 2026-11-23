#include "AppController.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

AppController::AppController(const std::vector<const CelestialBody*>& bodies)
    : gravity3_(1.0), gravity4_(1.0), selectedIntegrator_(&verlet_),
      methodName_("Verlet"), paused_(false), running_(true) {
    if (bodies.size() < 2) {
        throw std::invalid_argument("Scenario needs at least two bodies");
    }
    for (const CelestialBody* body : bodies) {
        if (body == nullptr) throw std::invalid_argument("Null scenario body");
        if (body->position().dimension() != bodies[0]->position().dimension()) {
            throw std::invalid_argument("Mixed scenario dimensions");
        }
        initialBodies_.push_back(body->clone());
    }
    startScenario(bodies[0]->position().dimension());
}

void AppController::startScenario(std::size_t dimension) {
    std::vector<std::unique_ptr<CelestialBody>> converted;
    std::vector<const CelestialBody*> input;
    for (const std::unique_ptr<CelestialBody>& initial : initialBodies_) {
        VectorND position(dimension);
        VectorND velocity(dimension);
        for (std::size_t axis = 0; axis < dimension; ++axis) {
            if (axis < initial->position().dimension()) {
                position[axis] = initial->position()[axis];
                velocity[axis] = initial->velocity()[axis];
            }
        }
        // Меняем общую физическую часть клона, сохраняя тип и внешний вид.
        std::unique_ptr<CelestialBody> body = initial->clone();
        body->setState(position, velocity);
        input.push_back(body.get());
        converted.push_back(std::move(body));
    }
    const GravityModel* gravity = &gravity3_;
    if (dimension == 4) gravity = &gravity4_;
    std::unique_ptr<Simulation> next = std::make_unique<Simulation>(
        input, *gravity, *selectedIntegrator_, 0.02);
    simulation_.swap(next);
    renderer_.clearTrails();
    renderer_.record(*simulation_);
}

const Simulation& AppController::simulation() const {
    return *simulation_;
}

void AppController::handleKey(char key) {
    switch (key) {
    case ' ': paused_ = !paused_; break;
    case 'r': case 'R':
        simulation_->reset();
        renderer_.clearTrails();
        renderer_.record(*simulation_);
        break;
    case '1': selectedIntegrator_ = &euler_; methodName_ = "Euler"; break;
    case '2': selectedIntegrator_ = &rk4_; methodName_ = "RK4"; break;
    case '3': selectedIntegrator_ = &verlet_; methodName_ = "Verlet"; break;
    case '+': case '=':
        if (renderer_.scale() * 1.1 <= 5.0) renderer_.setScale(renderer_.scale() * 1.1);
        break;
    case '-':
        if (renderer_.scale() / 1.1 >= 0.1) renderer_.setScale(renderer_.scale() / 1.1);
        break;
    case '[': renderer_.rotate4D(-0.1); break;
    case ']': renderer_.rotate4D(0.1); break;
    case 'd': case 'D': startScenario(simulation_->dimension() == 3 ? 4 : 3); break;
    case 'q': case 'Q': case 27: running_ = false; break;
    default: break;
    }
    simulation_->setIntegrator(*selectedIntegrator_);
}

void AppController::run(int frameLimit) {
    if (frameLimit < 0) throw std::invalid_argument("Frame limit cannot be negative");
    std::cout << "\x1b[2J";
    int frame = 0;
    while (running_ && (frameLimit == 0 || frame < frameLimit)) {
        if (frameLimit == 0) {
            while (_kbhit()) {
                int key = _getch();
                if (key == 0 || key == 224) {
                    _getch();
                } else {
                    handleKey(static_cast<char>(key));
                }
            }
        }
        if (!running_) break;
        renderer_.draw(*simulation_);
        std::cout << "Method: " << methodName_ << (paused_ ? " | PAUSED" : " | RUNNING")
                  << "\x1b[K" << std::flush;
        if (!paused_) {
            for (int step = 0; step < 5; ++step) simulation_->step();
            renderer_.record(*simulation_);
        }
        ++frame;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "\x1b[0m\n";
}
