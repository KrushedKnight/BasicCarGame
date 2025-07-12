//
// Created by beast-machine-2 on 7/6/25.
//

#include "Wheel.h"

Wheel::Wheel() : wheelAngle(0) {}

Eigen::Vector2d Wheel::calculateFriction(double carVelocity, double enginePower) {



    // angular_velocity += Constants::TIME_INTERVAL * (enginePower - frictionForce * Constants::WHEEL_RADIUS) / moment_of_inertia;
    //TODO: this needs to account for direction
    double slip = Constants::WHEEL_RADIUS * angular_velocity - carVelocity;

    double effective_mass = moment_of_inertia / (Constants::WHEEL_RADIUS * Constants::WHEEL_RADIUS);

    double requiredFrictionForce = (slip / Constants::TIME_INTERVAL) * effective_mass;
    double maxFrictionForce = normalForce * frictionCoefficient;
    double frictionForce = std::clamp(requiredFrictionForce, -maxFrictionForce, maxFrictionForce);


    if (slip == 0) {
        return Eigen::Vector2d::Zero();
    }



    double x = sin(wheelAngle) * frictionForce;
    double y = cos(wheelAngle) * frictionForce;


    Eigen::Vector2d friction{x, y};

    double frictionTorque = frictionForce * wheelRadius;
    addTorque(-frictionTorque);


    //TODO: this can be removed

    // if (angular_torque < 0) {
    //     angular_torque += (enginePower - frictionForce) * Constants::WHEEL_RADIUS;
    //     angular_torque = std::min(angular_torque, 0.0);
    // }
    // else if (angular_torque > 0) {
    //     angular_torque -= (enginePower - frictionForce) * Constants::WHEEL_RADIUS;
    //     angular_torque = std::max(angular_torque, 0.0);
    // }

    return friction;
}



double Wheel::getLinearVelocity() {
    return (angular_velocity*Constants::WHEEL_RADIUS);
}

void Wheel::setLinearVelocity(double linearVelocity) {
    angular_velocity = linearVelocity / Constants::WHEEL_RADIUS;
}

void Wheel::incrementTime(double time_interval) {
    angular_acceleration = angular_torque / moment_of_inertia;
    angular_position += angular_velocity * time_interval + 0.5 * angular_acceleration * time_interval * time_interval;
    angular_velocity += angular_acceleration * time_interval;

    clearTorques();
}