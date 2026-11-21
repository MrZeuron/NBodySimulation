#include "Gravity3D.h"

Gravity3D::Gravity3D(double gravitationalConstant)
    : PowerLawGravity(gravitationalConstant) {
}

std::size_t Gravity3D::dimension() const {
    return 3;
}
