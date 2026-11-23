#pragma once
#include "Projection4D.h"
#include "../simulation/Simulation.h"
#include <string>
#include <vector>

class Renderer {
public:
    Renderer();
    void setScale(double scale);
    double scale() const;
    void rotate4D(double angle);
    void clearTrails();
    void record(const Simulation& simulation);
    std::string frame(const Simulation& simulation) const;
    void draw(const Simulation& simulation) const;
private:
    static const int width = 61;
    static const int height = 17;
    double scale_;
    Projection4D projection_;
    std::vector<std::vector<VectorND>> trails_;

    bool project(const VectorND& position, int& column, int& row,
                 double& depth) const;
};
