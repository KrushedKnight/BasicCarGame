# BasicCarGame

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Dependencies](https://img.shields.io/badge/dependencies-SDL2%20%7C%20Eigen-orange)
![License](https://img.shields.io/badge/license-MIT-green)

A 2D rigid-body vehicle physics simulation built from scratch in C++. This project implements a custom physics engine to simulate realistic car dynamics, including friction, torque, and steering geometry, without relying on "black box" physics libraries like Box2D.

![Demo Placeholder](https://via.placeholder.com/800x400.png?text=Gameplay+Demo+Gif)
*(Placeholder for gameplay GIF)*

## Table of Contents
- [Problem Solved](#problem-solved)
- [Key Features](#key-features)
- [Architecture](#architecture)
- [Engineering Decisions](#engineering-decisions)
- [Tech Stack](#tech-stack)
- [Getting Started](#getting-started)
- [Future Improvements](#future-improvements)

## Problem Solved
Most game developers rely on pre-built engines (Unity, Unreal) or physics libraries (Box2D, PhysX) which abstract away the underlying mathematics of motion. This project solves the problem of **"black box" physics dependency** by implementing the core equations of motion from first principles. It demonstrates a deep understanding of:
- **Newtonian Mechanics**: Integration of forces and torques to derive velocity and position.
- **Vehicle Dynamics**: Modeling wheel friction and steering geometry manually.
- **Vector Mathematics**: Applied linear algebra for 2D space simulation.

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

### Code Snippet: Friction Calculation
One of the most challenging parts was calculating the correct friction vector for a drifting wheel:
```cpp
// Wheel.cpp
Eigen::Vector2d Wheel::calculateFriction(Eigen::Vector2d carVelocity) {
    // Calculate velocity relative to tire patch
    Eigen::Vector2d patchVel = carVelocity + ...; 
    
    // Decompose into lateral and forward components
    Eigen::Vector2d lateralDir = ...;
    double lateralVel = patchVel.dot(lateralDir);

    // Apply friction coefficient opposing motion
    return -lateralDir * lateralVel * frictionCoefficient;
}
```

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

## Future Improvements
*   **Collision Detection**: Implementing SAT (Separating Axis Theorem) for car-to-obstacle collisions.
*   **Integration Method**: Upgrading from implicit Euler to RK4 integration for higher precision at lower framerates.
*   **Render Interpolation**: Decoupling physics tick rate from render framerate for smoother visuals.

---
*Built by KrushedKnight*
