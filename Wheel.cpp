//
// Created by beast-machine-2 on 7/6/25.
//

#include "Wheel.h"

Wheel::Wheel() : wheelAngle(0) {}

Eigen::Vector2d Wheel::calculateFriction() {
    // double x = sin(wheelAngle) * normalForce * frictionCoefficient;
    // double y = cos(wheelAngle) * normalForce * frictionCoefficient;
    // return Eigen::Vector2d{x, y};
    Eigen::Vector2d friction{0, normalForce * frictionCoefficient};
    return friction;
}
