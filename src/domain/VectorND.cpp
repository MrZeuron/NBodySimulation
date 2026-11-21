#include "VectorND.h"

#include <cmath>
#include <stdexcept>

VectorND::VectorND(std::size_t dimension)
    : dimension_(dimension) {
    requireValidDimension(dimension);
    components_.fill(0.0);
}

VectorND::VectorND(std::size_t dimension, double value)
    : dimension_(dimension) {
    requireValidDimension(dimension);
    components_.fill(value);
}

VectorND VectorND::zero3() {
    return VectorND(3);
}

VectorND VectorND::zero4() {
    return VectorND(4);
}

std::size_t VectorND::dimension() const {
    return dimension_;
}

double& VectorND::operator[](std::size_t index) {
    if (index >= dimension_) {
        throw std::out_of_range("Vector index is out of range");
    }
    return components_[index];
}

double VectorND::operator[](std::size_t index) const {
    if (index >= dimension_) {
        throw std::out_of_range("Vector index is out of range");
    }
    return components_[index];
}

VectorND VectorND::operator+(const VectorND& other) const {
    requireMatchingDimension(other);

    VectorND result(dimension());
    for (std::size_t i = 0; i < dimension(); ++i) {
        result[i] = components_[i] + other[i];
    }
    return result;
}

VectorND VectorND::operator-(const VectorND& other) const {
    requireMatchingDimension(other);

    VectorND result(dimension());
    for (std::size_t i = 0; i < dimension(); ++i) {
        result[i] = components_[i] - other[i];
    }
    return result;
}

VectorND VectorND::operator*(double scalar) const {
    VectorND result(dimension());
    for (std::size_t i = 0; i < dimension(); ++i) {
        result[i] = components_[i] * scalar;
    }
    return result;
}

VectorND& VectorND::operator+=(const VectorND& other) {
    requireMatchingDimension(other);

    for (std::size_t i = 0; i < dimension(); ++i) {
        components_[i] += other[i];
    }
    return *this;
}

VectorND& VectorND::operator-=(const VectorND& other) {
    requireMatchingDimension(other);

    for (std::size_t i = 0; i < dimension(); ++i) {
        components_[i] -= other[i];
    }
    return *this;
}

double VectorND::dot(const VectorND& other) const {
    requireMatchingDimension(other);

    double result = 0.0;
    for (std::size_t i = 0; i < dimension(); ++i) {
        result += components_[i] * other[i];
    }
    return result;
}

double VectorND::squaredNorm() const {
    return dot(*this);
}

double VectorND::norm() const {
    return std::sqrt(squaredNorm());
}

void VectorND::requireValidDimension(std::size_t dimension) const {
    if (dimension != 3 && dimension != 4) {
        throw std::invalid_argument("VectorND supports only 3 or 4 dimensions");
    }
}

void VectorND::requireMatchingDimension(const VectorND& other) const {
    if (dimension() != other.dimension()) {
        throw std::invalid_argument("Vector dimensions do not match");
    }
}
