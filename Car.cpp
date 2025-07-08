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
    acceleration = acceleration;
}


const int Car::getWidth() {
    return width;
}

const int Car::getHeight() {
    return height;
}

void Car::applySteering() {
    frontLeft->WHEEL_ANGLE = steering_angle * Constants::STEERING_RACK;
    frontRight->WHEEL_ANGLE = steering_angle * Constants::STEERING_RACK;
}

void Car::applyEngineTorque() {
    backRight->addTorque(backRight->WHEEL_RADIUS * engine_power);
    backLeft->addTorque(backLeft->WHEEL_RADIUS * engine_power);
}

void Car::sumWheelForces() {
    for (int i = 0; i < wheels.size(); i++) {
        //add some grip so that these wheels pull the car by a certain amount
        // wheels[i]->WHEEL_ANGLE
    }
}

void Car::drawCar(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect rect{getPositionX(), getPositionY(), getWidth(), getHeight()};


    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void Car::eraseCar(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}