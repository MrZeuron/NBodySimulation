#include "Projection4D.h"
#include <cmath>
#include <stdexcept>

Projection4D::Projection4D(double angle) {
    setAngle(angle);
}

void Projection4D::setAngle(double angle) {
    if (!std::isfinite(angle)) {
        throw std::invalid_argument("Projection angle must be finite");
    }
    angle_ = angle;
}

double Projection4D::angle() const {
    return angle_;
}

VectorND Projection4D::project(const VectorND& position) const {
    VectorND result(3);
    result[0] = position[0];
    result[1] = position[1];
    result[2] = position[2];
    if (position.dimension() == 4) {
        result[0] = position[0] * std::cos(angle_)
                    - position[3] * std::sin(angle_);
    }
    return result;
}
