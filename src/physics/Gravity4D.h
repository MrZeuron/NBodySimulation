#pragma once

#include "PowerLawGravity.h"

class Gravity4D : public PowerLawGravity {
public:
    explicit Gravity4D(double gravitationalConstant);
    std::size_t dimension() const override;
};
