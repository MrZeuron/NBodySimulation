#pragma once
#include "VectorND.h"

// Описывает одно небесное тело и его текущее состояние.
class CelestialBody {
public:
    // Конструктор задаёт массу, положение и начальную скорость.
    CelestialBody(double mass, const VectorND& position, const VectorND& velocity);

    // Безопасно читают данные тела.
    double mass() const;
    const VectorND& position() const;
    const VectorND& velocity() const;

    // Изменяют параметры с проверкой допустимости данных.
    void setMass(double mass);
    void setPosition(const VectorND& position);
    void setVelocity(const VectorND& velocity);

private:
    // Данные, которые принадлежат одному конкретному телу.
    double mass_;
    VectorND position_;
    VectorND velocity_;

    // Внутренние правила поддерживают корректное состояние объекта.
    void requireValidMass(double mass) const;
    void requireFinite(const VectorND& vector) const;
};
