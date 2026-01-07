# BasicCarGame

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Dependencies](https://img.shields.io/badge/dependencies-SDL2%20%7C%20Eigen-orange)
![License](https://img.shields.io/badge/license-MIT-green)

A 2D rigid-body vehicle physics simulation built from scratch in C++. This project implements a custom physics engine to simulate realistic car dynamics, including friction, torque, and steering geometry, without relying on "black box" physics libraries like Box2D.

![Demo Placeholder](https://via.placeholder.com/800x400.png?text=Gameplay+Demo+Gif)
*(Placeholder for gameplay GIF)*

## Table of Contents
- [Project Motivation](#project-motivation)
- [Key Features](#key-features)
- [Architecture](#architecture)
- [Engineering Decisions](#engineering-decisions)
- [Tech Stack](#tech-stack)
- [Getting Started](#getting-started)
- [Future Improvements](#future-improvements)

## Project Motivation
This project was built as an exploration of vehicle dynamics and real-time physics simulation, rather than to solve a specific end-user problem.

The primary goals were to:
- Understand rigid body dynamics beyond black-box physics engines
- Implement vehicle motion from first principles
- Experiment with stability, friction, and steering behavior in a controlled environment

By avoiding full-featured engines like Unity or physics libraries like Box2D, the project focuses on **how** car physics actually work—from force accumulation to numerical integration.

## Key Features
- **Custom Rigid Body Physics Engine**: A lightweight physics core covering linear/angular velocity, acceleration, mass, and moment of inertia.
- **Realistic Friction Model**: Each wheel independently calculates static and kinetic friction based on surface vectors.
- **Ackermann-style Steering**: Simplified steering geometry for realistic turning behavior.
- **Interactive Control**: Real-time control via Keyboard or Joystick using SDL2.
- **Visualization**: Rendering of vehicle state and wheel positions.

## Architecture
The simulation runs on a custom **fixed-timestep loop** that ensures deterministic physics behavior.

### Core Components
*   **`RigidBody`**: The base class for all physical objects. It handles the state integration (position, velocity, acceleration) and force accumulation.
*   **`Wheel`**: Inherits from `RigidBody`. It adds logic for friction calculation—determining how much grip the tire has based on its angle relative to movement.
*   **`Car`**: The composite entity. It aggregates four `Wheel` objects and a chassis, managing the distribution of engine torque and transfer of input forces.

### Simulation Loop
1.  **Input Polling**: SDL2 captures keyboard/controller state.
2.  **Force Application**: Engine torque and steering angles are applied to wheels.
3.  **Physics Step**:
    *   Calculate friction vectors for all wheels.
    *   Sum forces and torques on the chassis.
    *   Integrate acceleration $\rightarrow$ velocity $\rightarrow$ position.
4.  **Render**: Clear screen and draw current state.

## Engineering Decisions

### Why Custom Physics instead of Box2D?
I chose to write the physics engine from scratch to gain a fundamental understanding of **numerical integration** and **rigid body dynamics**. While Box2D is more robust, building this manually taught me how to stabilize a simulation and handle edge cases like friction impulse.

### Using Eigen for Math
Instead of writing a custom `Vector2` class, I utilized the **Eigen** library.
*   **Reason**: Eigen provides optimized, SIMD-friendly vector operations. It reduces the likelihood of math errors in dot products, normalizations, and matrix operations, allowing me to focus on the physics logic rather than the math library implementation.

### Component-Based Design
The car is not a single monolithic object but a composition of parts (`Car` has `Wheels`). This allows for modular testing of individual components. For example, a `Wheel` can be tested in isolation to verify friction calculations before being attached to a `Car`.

## Testing: Deterministic Validation
A core design goal of this project is **determinism**. The physics engine is structured so that identical initial conditions and identical input sequences always produce identical results across runs, independent of rendering frame rate or machine performance.

This deterministic behavior enables rigorous testing and precise debugging—both of which are critical for physics-heavy systems.

### Why Determinism Matters
Deterministic simulation allows:
- Step-by-step validation of physics behavior
- Reliable regression testing when modifying friction or integration logic
- Exact reproduction of bugs and instability scenarios
- Comparison of intermediate values (forces, velocities, torques) across runs

This design mirrors approaches used in robotics simulators and game engines where reproducibility is essential for validation.

### Testing Approach
The testing framework is built around **Google Test** and focuses on validating physics correctness under controlled conditions.

Key principles:
- **Fixed timestep integration** ensures time consistency
- **No frame-dependent physics updates** (rendering is fully decoupled)
- **Repeatable input sequences** drive deterministic outcomes
- Physics state is advanced in isolation from rendering or user input

Tests target both individual components and system-level behavior, including:
- Force accumulation correctness
- Friction direction and magnitude under slip
- Velocity and angular velocity integration
- Steering response under controlled inputs

This approach made it possible to isolate subtle issues such as:
- Energy gain/loss due to integration error
- Incorrect lateral friction direction at high slip angles
- Steering instability at low speeds


## Tech Stack
*   **Language**: C++17
*   **Math Library**: Eigen 3.4.0
*   **Graphics/Input**: SDL2
*   **Build System**: CMake

## Getting Started

### Prerequisites
*   C++ Compiler (GCC/Clang/MSVC)
*   CMake 3.10+
*   SDL2 Development Libraries

### Installation
1.  **Clone the repository**:
    ```bash
    git clone https://github.com/KrushedKnight/BasicCarGame.git
    cd BasicCarGame
    ```
2.  **Build using CMake**:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```
3.  **Run the simulation**:
    ```bash
    ./SimpleTrafficGame
    ```
### Running the Tests
This project uses **Google Test** for unit and integration testing.

#### Build and Run Tests
From the project root:

```bash
mkdir build && cd build
cmake ..
make
ctest
```

## Future Improvements
*   **Collision Detection**: Implementing SAT (Separating Axis Theorem) for car-to-obstacle collisions.
*   **Integration Method**: Upgrading from implicit Euler to RK4 integration for higher precision at lower framerates.
*   **Render Interpolation**: Decoupling physics tick rate from render framerate for smoother visuals.

---
*Built by KrushedKnight*
