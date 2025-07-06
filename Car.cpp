//
// Created by beast-machine-2 on 7/2/25.
//

#include "Car.h"

Car::Car(double x, double y, int w, int h)
    : pos_x(x), pos_y(y), velocity(Eigen::Vector2d::Zero()), acceleration(Eigen::Vector2d::Zero()),
      width(w),
      height(h) {}


void Car::applyInput(Eigen::Vector2d &inputVector) {
    this->acceleration = acceleration;
}

int Car::getPositionX() {
    return std::floor(this->pos_x);
}

int Car::getPositionY() {
    return std::floor(this->pos_y);
}

void Car::setPositionX(double x) {
    this->pos_x = x;
}

void Car::setPositionY(double y) {
    this->pos_y = y;
}


const int Car::getWidth() {
    return this->width;
}

const int Car::getHeight() {
    return this->height;
}

void Car::incrementTime(double time_step) {
    this->setPositionX(this->pos_x + this->velocity.x() * time_step + 0.5 * this->acceleration.x() * time_step * time_step);
    this->setPositionY(this->pos_y + this->velocity.y() * time_step + 0.5 * this->acceleration.y() * time_step * time_step);

    this->velocity = this->velocity + this->acceleration * time_step;
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