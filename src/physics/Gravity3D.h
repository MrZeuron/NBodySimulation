#pragma once

#include "PowerLawGravity.h"

class Gravity3D : public PowerLawGravity {
public:
    explicit Gravity3D(double gravitationalConstant);
    std::size_t dimension() const override;
};
