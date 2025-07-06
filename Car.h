//
// Created by beast-machine-2 on 7/2/25.
//

#ifndef CAR_H
#define CAR_H
#include <SDL_rect.h>
#include <SDL_render.h>
#include <eigen-3.4.0/Eigen/Dense>


class Car {
    public:
        double pos_x;
        double pos_y;

        Eigen::Vector2d velocity;
        Eigen::Vector2d acceleration;
        Car(double x, double y, int w, int h);


        void setAcceleration(Eigen::Vector2d &acceleration);

        int getPositionX();
        int getPositionY();

        const int getWidth();
        const int getHeight();

        void setPositionX(double x);
        void setPositionY(double y);

        void incrementTime(double time_step);

        void drawCar(SDL_Renderer* renderer);

        void eraseCar(SDL_Renderer *renderer);

    private:
        const double width;
        const double height;
};



#endif //CAR_H
