//
// Created by beast-machine-2 on 7/2/25.
//

#include "Car.h"



Car::Car(double x, double y, int w, int h) : width(w), height(h), steering_angle(0){
    pos_x = x;
    pos_y = y;

    frontLeft = new Wheel();
    frontRight = new Wheel();
    backLeft = new Wheel();
    backRight = new Wheel();

}


void Car::applyInput(Eigen::Vector2d &inputVector) {
    this->acceleration = acceleration;
}


const int Car::getWidth() {
    return this->width;
}

const int Car::getHeight() {
    return this->height;
}

void Car::drawCar(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect rect{this->getPositionX(), this->getPositionY(), this->getWidth(), this->getHeight()};


    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void Car::eraseCar(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}