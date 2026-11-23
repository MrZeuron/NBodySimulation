#pragma once
#include "VectorND.h"
#include "Appearance.h"
#include <memory>
#include <string>

// Описывает одно небесное тело и его текущее состояние.
class CelestialBody {
public:
    // Конструктор задаёт массу, положение и начальную скорость.
    CelestialBody(double mass, const VectorND& position, const VectorND& velocity);
    virtual ~CelestialBody();
    virtual std::unique_ptr<CelestialBody> clone() const;
    virtual Appearance appearance() const;
    virtual const char* typeName() const;
    const std::string& name() const;
    void setName(const std::string& name);

    // Безопасно читают данные тела.
    double mass() const;
    const VectorND& position() const;
    const VectorND& velocity() const;

    // Изменяют параметры с проверкой допустимости данных.
    void setMass(double mass);
    void setPosition(const VectorND& position);
    void setVelocity(const VectorND& velocity);
    // Атомарно задаёт пару векторов, в том числе при смене 3D/4D-сценария.
    void setState(const VectorND& position, const VectorND& velocity);

private:
    std::string name_;
    // Данные, которые принадлежат одному конкретному телу.
    double mass_;
    VectorND position_;
    VectorND velocity_;

    // Внутренние правила поддерживают корректное состояние объекта.
    void requireValidMass(double mass) const;
    void requireFinite(const VectorND& vector) const;
};
