#pragma once

#include "GravityModel.h"

class Gravity4D : public GravityModel {
public:
    explicit Gravity4D(double gravitationalConstant);
    std::size_t dimension() const override;
};
