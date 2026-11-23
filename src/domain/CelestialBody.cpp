#include "CelestialBody.h"

#include <cmath>
#include <stdexcept>

CelestialBody::~CelestialBody() {
}

std::unique_ptr<CelestialBody> CelestialBody::clone() const {
    return std::make_unique<CelestialBody>(*this);
}

Appearance CelestialBody::appearance() const {
    return Appearance();
}

const char* CelestialBody::typeName() const {
    return "Body";
}

const std::string& CelestialBody::name() const {
    return name_;
}

void CelestialBody::setName(const std::string& name) {
    if (name.size() > 40 || name.find_first_of("\r\n\t\x1b") != std::string::npos) {
        throw std::invalid_argument("Body name must be short and contain no terminal controls");
    }
    for (unsigned char character : name) {
        if (character < 32 || character == 127) {
            throw std::invalid_argument("Body name contains a control character");
        }
    }
    name_ = name;
}

// Создаёт тело и сразу проверяет все физические и геометрические данные.
CelestialBody::CelestialBody(double mass,
                             const VectorND& position,
                             const VectorND& velocity)
    : mass_(mass),
      position_(position),
      velocity_(velocity) {
    requireValidMass(mass);
    requireFinite(position);
    requireFinite(velocity);

    if (position.dimension() != velocity.dimension()) {
        throw std::invalid_argument("Position and velocity dimensions do not match");
    }
}

// Геттеры читают состояние тела без передачи возможности изменить массу или векторы.
double CelestialBody::mass() const {
    return mass_;
}

const VectorND& CelestialBody::position() const {
    return position_;
}

const VectorND& CelestialBody::velocity() const {
    return velocity_;
}

// Сеттеры обновляют состояние только после успешных проверок.
void CelestialBody::setMass(double mass) {
    requireValidMass(mass);
    mass_ = mass;
}

void CelestialBody::setPosition(const VectorND& position) {
    requireFinite(position);
    if (position.dimension() != position_.dimension()) {
        throw std::invalid_argument("Position dimension does not match the body");
    }
    position_ = position;
}

void CelestialBody::setVelocity(const VectorND& velocity) {
    requireFinite(velocity);
    if (velocity.dimension() != velocity_.dimension()) {
        throw std::invalid_argument("Velocity dimension does not match the body");
    }
    velocity_ = velocity;
}

void CelestialBody::setState(const VectorND& position, const VectorND& velocity) {
    requireFinite(position);
    requireFinite(velocity);
    if (position.dimension() != velocity.dimension()) {
        throw std::invalid_argument("State vector dimensions do not match");
    }
    position_ = position;
    velocity_ = velocity;
}

// Масса должна быть конечным положительным числом.
void CelestialBody::requireValidMass(double mass) const {
    if (!std::isfinite(mass) || mass <= 0.0) {
        throw std::invalid_argument("Mass must be finite and positive");
    }
}

// Запрет NaN и бесконечности предотвращает некорректный физический расчёт.
void CelestialBody::requireFinite(const VectorND& vector) const {
    for (std::size_t i = 0; i < vector.dimension(); ++i) {
        if (!std::isfinite(vector[i])) {
            throw std::invalid_argument("Vector components must be finite");
        }
    }
}
