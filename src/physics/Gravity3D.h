#pragma once

#include "GravityModel.h"

class Gravity3D : public GravityModel {
public:
    explicit Gravity3D(double gravitationalConstant);
    std::size_t dimension() const override;
};
