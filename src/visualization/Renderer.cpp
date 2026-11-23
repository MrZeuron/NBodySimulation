#include "Renderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
VectorND point(double x, double y, double z) {
    VectorND result(3);
    result[0] = x;
    result[1] = y;
    result[2] = z;
    return result;
}

std::string colorCode(BodyColor color) {
    return "\x1b[38;2;" + std::to_string(color.red) + ";"
           + std::to_string(color.green) + ";" + std::to_string(color.blue) + "m";
}
}

Renderer::Renderer() : scale_(1.0) {
}

void Renderer::setScale(double scale) {
    if (!std::isfinite(scale) || scale < 0.1 || scale > 5.0) {
        throw std::invalid_argument("View scale must be between 0.1 and 5");
    }
    scale_ = scale;
}

double Renderer::scale() const {
    return scale_;
}

void Renderer::rotate4D(double angle) {
    projection_.setAngle(projection_.angle() + angle);
}

void Renderer::clearTrails() {
    trails_.clear();
}

void Renderer::record(const Simulation& simulation) {
    trails_.resize(simulation.bodyCount());
    for (std::size_t i = 0; i < trails_.size(); ++i) {
        trails_[i].push_back(simulation.body(i).position());
        if (trails_[i].size() > 100) {
            trails_[i].erase(trails_[i].begin());
        }
    }
}

bool Renderer::project(const VectorND& position, int& column, int& row,
                       double& depth) const {
    VectorND view = projection_.project(position);
    double x = width / 2.0 + (view[0] - view[1]) * 0.9 * scale_;
    double y = height / 2.0 + ((view[0] + view[1]) * 0.45 - view[2] * 0.9) * scale_;
    depth = view[0] + view[1] + view[2];
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(depth)
        || x < 0.0 || x >= width || y < 0.0 || y >= height) {
        return false;
    }
    column = static_cast<int>(x);
    row = static_cast<int>(y);
    return true;
}

std::string Renderer::frame(const Simulation& simulation) const {
    std::string screen[height];
    BodyColor colors[height][width];
    double depths[height][width];
    for (int row = 0; row < height; ++row) {
        screen[row] = std::string(width, ' ');
        for (int column = 0; column < width; ++column) {
            colors[row][column] = {85, 90, 110};
            depths[row][column] = -std::numeric_limits<double>::infinity();
        }
    }

    int column, row;
    double depth;
    for (int x = -20; x <= 20; x += 5) {
        for (int y = -20; y <= 20; y += 5) {
            if (project(point(x, y, 0), column, row, depth)) {
                screen[row][column] = '.';
            }
        }
    }
    for (int distance = -14; distance <= 14; ++distance) {
        if (project(point(distance, 0, 0), column, row, depth)) screen[row][column] = '\\';
        if (project(point(0, distance, 0), column, row, depth)) screen[row][column] = '/';
    }
    for (int z = -7; z <= 7; ++z) {
        if (project(point(0, 0, z), column, row, depth)) screen[row][column] = '|';
    }
    if (project(point(16, 0, 0), column, row, depth)) screen[row][column] = 'X';
    if (project(point(0, 16, 0), column, row, depth)) screen[row][column] = 'Y';
    if (project(point(0, 0, 8), column, row, depth)) screen[row][column] = 'Z';

    // Траектории хранятся в исходной размерности и проецируются заново при повороте.
    for (const std::vector<VectorND>& trail : trails_) {
        for (const VectorND& position : trail) {
            if (project(position, column, row, depth)) {
                screen[row][column] = ':';
                colors[row][column] = {100, 130, 160};
            }
        }
    }

    for (std::size_t i = 0; i < simulation.bodyCount(); ++i) {
        const CelestialBody& body = simulation.body(i);
        if (!project(body.position(), column, row, depth)) continue;
        Appearance appearance = body.appearance();
        int radius = static_cast<int>(std::clamp(appearance.radius * scale_, 0.0, 3.0));
        int halo = appearance.glow > 0.0 ? 1 : 0;
        int extent = radius + halo;
        for (int dy = -extent; dy <= extent; ++dy) {
            for (int dx = -extent * 2; dx <= extent * 2; ++dx) {
                int targetX = column + dx;
                int targetY = row + dy;
                if (targetX < 0 || targetX >= width || targetY < 0 || targetY >= height) continue;
                // Символ в терминале примерно вдвое выше своей ширины.
                double distance = std::sqrt(dx * dx / 4.0 + dy * dy);
                if (distance > extent || depth < depths[targetY][targetX]) continue;
                char symbol = appearance.symbol;
                BodyColor color = appearance.color;
                if (distance > radius) {
                    symbol = '.';
                    double brightness = std::min(0.65, 0.2 + appearance.glow * 0.15);
                    color = {static_cast<int>(color.red * brightness),
                             static_cast<int>(color.green * brightness),
                             static_cast<int>(color.blue * brightness)};
                } else if (dx != 0 || dy != 0) {
                    switch (appearance.surface) {
                    case SurfaceStyle::Banded:
                        symbol = dy % 2 == 0 ? '=' : '-';
                        if (dy % 2 == 0) {
                            color.red /= 2; color.green /= 2; color.blue /= 2;
                        }
                        break;
                    case SurfaceStyle::Rocky: symbol = '#'; break;
                    case SurfaceStyle::Cratered: symbol = 'o'; break;
                    case SurfaceStyle::Irregular:
                        if ((dx + dy) % 3 == 0) continue;
                        symbol = '*';
                        break;
                    case SurfaceStyle::Plain: symbol = '@'; break;
                    }
                }
                screen[targetY][targetX] = symbol;
                colors[targetY][targetX] = color;
                depths[targetY][targetX] = depth;
            }
        }
        // Номер связывает объект на поле с подписью, даже если виды совпадают.
        std::string label = std::to_string(i + 1);
        int labelX = column + radius * 2 + 1;
        for (std::size_t j = 0; j < label.size(); ++j) {
            int x = labelX + static_cast<int>(j);
            if (x < width && depth >= depths[row][x]) {
                screen[row][x] = label[j];
                colors[row][x] = appearance.color;
                depths[row][x] = depth;
            }
        }
    }

    std::ostringstream output;
    output << "\x1b[H" << simulation.dimension() << "D ASCII scene | X/Y floor, Z up\x1b[K\n";
    output << "t=" << simulation.time() << " dt=" << simulation.timeStep()
           << " E=" << simulation.totalEnergy() << " zoom=" << scale_ << "\x1b[K\n";
    output << '+' << std::string(width, '-') << "+\x1b[K\n";
    for (int y = 0; y < height; ++y) {
        output << '|';
        for (int x = 0; x < width; ++x) {
            output << colorCode(colors[y][x]) << screen[y][x];
        }
        output << "\x1b[0m|\x1b[K\n";
    }
    output << '+' << std::string(width, '-') << "+\x1b[K\n";
    for (std::size_t i = 0; i < simulation.bodyCount(); ++i) {
        const CelestialBody& body = simulation.body(i);
        output << colorCode(body.appearance().color) << body.appearance().symbol
               << "\x1b[0m " << i + 1 << ": " << body.typeName();
        if (!body.name().empty()) output << " [" << body.name() << ']';
        output << "\x1b[K\n";
    }
    output << "Space pause | R reset | 1 Euler 2 RK4 3 Verlet\x1b[K\n"
           << "+/- zoom | D 3D/4D (restart) | [/] 4D view | Q quit\x1b[K\n\x1b[J";
    return output.str();
}

void Renderer::draw(const Simulation& simulation) const {
    std::cout << frame(simulation) << std::flush;
}
