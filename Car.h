//
// Created by beast-machine-2 on 7/2/25.
//

#ifndef CAR_H
#define CAR_H
#include <SDL_rect.h>
#include <SDL_render.h>
#include <eigen-3.4.0/Eigen/Dense>

#include "RigidBody.h"


class Car : public RigidBody {
    public:

        Car(double x, double y, int w, int h);



        void applyInput(Eigen::Vector2d &acceleration);


        const int getWidth();
        const int getHeight();


        void drawCar(SDL_Renderer* renderer);
        void eraseCar(SDL_Renderer *renderer);

    private:
        const double width;
        const double height;
};



#endif //CAR_H
