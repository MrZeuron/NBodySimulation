#include "Gravity4D.h"

Gravity4D::Gravity4D(double gravitationalConstant)
    : GravityModel(gravitationalConstant) {
}

std::size_t Gravity4D::dimension() const {
    return 4;
}
