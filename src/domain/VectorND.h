#pragma once

#include <array>
#include <cstddef>

// Вектор из трёх или четырёх координат.
class VectorND {
public:
    explicit VectorND(std::size_t dimension);
    VectorND(std::size_t dimension, double value);

    static VectorND zero3();
    static VectorND zero4();

    std::size_t dimension() const;
    double& operator[](std::size_t index);
    double operator[](std::size_t index) const;

    VectorND operator+(const VectorND& other) const;
    VectorND operator-(const VectorND& other) const;
    VectorND operator*(double scalar) const;
    VectorND& operator+=(const VectorND& other);
    VectorND& operator-=(const VectorND& other);

    double dot(const VectorND& other) const;
    double squaredNorm() const;
    double norm() const;

private:
    std::array<double, 4> components_;
    std::size_t dimension_;

    void requireValidDimension(std::size_t dimension) const;
    void requireMatchingDimension(const VectorND& other) const;
};
