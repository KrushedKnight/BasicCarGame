#include "rendering/Camera.h"

Camera::Camera(double x, double y, double damping)
    : camera_x(x), camera_y(y), damping_factor(damping) {}

void Camera::followTargetSmooth(double target_x, double target_y) {
    camera_x += (target_x - camera_x) * damping_factor;
    camera_y += (target_y - camera_y) * damping_factor;
}

void Camera::setDamping(double damping) {
    damping_factor = damping;
}

int Camera::worldToScreenX(double world_x, int screen_width) const {
    return static_cast<int>(world_x - camera_x + screen_width / 2);
}

int Camera::worldToScreenY(double world_y, int screen_height) const {
    return static_cast<int>(world_y - camera_y + screen_height / 2);
}