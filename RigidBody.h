//
// Created by beast-machine-2 on 7/6/25.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <eigen-3.4.0/Eigen/src/Core/Matrix.h>


class RigidBody {
public:
    double pos_x;
    double pos_y;

    Eigen::Vector2d velocity;
    Eigen::Vector2d acceleration;
    Eigen::Vector2d forces;

    double angular_position; //from -180 to 180
    double angular_velocity;
    double angular_acceleration;
    double angular_torque;

    //TODO move constants

    double mass;
    double moment_of_inertia;

    RigidBody();

    int getPositionX();
    int getPositionY();


    void addForce(Eigen::Vector2d force);
    void addTorque(Eigen::Vector2d torque);

    void clearForces();
    void clearTorques();

    Eigen::Vector2d sumForces();
    double sumTorques();

    void incrementTime(double time_interval);
};



#endif //RIGIDBODY_H
