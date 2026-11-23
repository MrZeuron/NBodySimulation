#pragma once

// Описание внешнего вида без зависимости от графической библиотеки.
struct BodyColor {
    int red;
    int green;
    int blue;
};

enum class SurfaceStyle { Plain, Rocky, Banded, Cratered, Irregular };

struct Appearance {
    char symbol = 'o';
    BodyColor color = {200, 200, 200};
    double radius = 0.5;
    double glow = 0.0;
    SurfaceStyle surface = SurfaceStyle::Plain;
};
