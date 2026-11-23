#pragma once
#include "../integrators/EulerIntegrator.h"
#include "../integrators/RK4Integrator.h"
#include "../integrators/VerletIntegrator.h"
#include "../physics/Gravity3D.h"
#include "../physics/Gravity4D.h"
#include "../visualization/Renderer.h"

class AppController {
public:
    explicit AppController(const std::vector<const CelestialBody*>& bodies);
    void run(int frameLimit = 0);
    void handleKey(char key);
    const Simulation& simulation() const;
private:
    // Поля модели и интеграторов живут дольше симуляции.
    Gravity3D gravity3_;
    Gravity4D gravity4_;
    EulerIntegrator euler_;
    RK4Integrator rk4_;
    VerletIntegrator verlet_;
    const Integrator* selectedIntegrator_;
    const char* methodName_;
    std::vector<std::unique_ptr<CelestialBody>> initialBodies_;
    std::unique_ptr<Simulation> simulation_;
    Renderer renderer_;
    bool paused_;
    bool running_;

    void startScenario(std::size_t dimension);
};
