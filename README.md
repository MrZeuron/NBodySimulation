# NBodySimulation

A small C++20 simulator for gravitational interaction of multiple bodies in 3D
and 4D Euclidean space. The current renderer is an ANSI-colored ASCII scene in
the terminal. It uses only the C++ standard library and Windows console input.

## Build and run

Use GCC (g++) and a Windows terminal with ANSI color support. From the project
root, build all sources with:

```powershell
$sources = Get-ChildItem src -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
g++ -std=c++20 -Wall -Wextra -pedantic -Isrc/domain -Isrc/physics -Isrc/integrators -Isrc/simulation -Isrc/visualization -Isrc/app @sources main.cpp -o simulator.exe
```

Run the two-body scenario, or add `--demo` to show every body type. Add `--4d`
to start in four dimensions. `--frames N` runs a limited number of frames,
which is useful for a quick smoke test.

```powershell
./simulator.exe
./simulator.exe --demo
./simulator.exe --demo --4d --frames 30
```

The initial masses, coordinates, and velocities are configured in `main.cpp`.
The view displays time, total energy, short trajectories, labels, and different
symbols/colors for stars, planets, moons, and asteroids.

## Controls

- `Space`: pause/resume
- `R`: reset to the starting state
- `1`, `2`, `3`: Euler, RK4, or Verlet integration
- `+`, `-`: zoom the ASCII view
- `D`: restart the same scenario in 3D or 4D
- `[`, `]`: rotate the X/W projection in 4D
- `Q` or `Esc`: quit

## Project layout

- `src/domain`: vectors, physical bodies, and appearance descriptions
- `src/physics`: 3D and 4D gravity models
- `src/integrators`: Euler, RK4, and velocity Verlet methods
- `src/simulation`: simulation state, stepping, and reset
- `src/visualization`: ASCII renderer and 4D projection
- `src/app`: keyboard controls and application loop
- `tests`: small executable checks for the components

Every body participates in the same gravity calculation. Body subclasses
provide visual appearance data; these properties do not affect the physics.
`Simulation` owns polymorphic body copies and borrows its gravity model and
integrator, which must outlive it.

## Tests

The test sources are standalone programs. Compile each test with its required
project sources. `tests/architecture_test.cpp` covers body polymorphism,
integrator/model switching, reset, and 3D/4D projection behavior.

The equations and interpretation of the 4D model are described in the project
documentation used for the coursework.
