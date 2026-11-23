#pragma once
#include "../domain/VectorND.h"

// Поворот X/W и ортографическая проекция; расчётные координаты не меняются.
class Projection4D {
public:
    explicit Projection4D(double angle = 0.6);
    void setAngle(double angle);
    double angle() const;
    VectorND project(const VectorND& position) const;
private:
    double angle_;
};
