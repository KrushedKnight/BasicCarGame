//
// Created by beast-machine-2 on 7/6/25.
//

#ifndef WHEEL_H
#define WHEEL_H

#include "constants.h"
#include "RigidBody.h"


class Wheel : public RigidBody {
public:
    double WHEEL_ANGLE;

    double WHEEL_RADIUS{Constants::WHEEL_RADIUS};

    double frictionCoefficient{Constants::WHEEL_FRICTION};

    Wheel();
};



#endif //WHEEL_H
