#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    double camera_x;
    double camera_y;
    double damping_factor;

    Camera(double x, double y, double damping = 0.1);

    void followTargetSmooth(double target_x, double target_y);
    void setDamping(double damping);

    int worldToScreenX(double world_x, int screen_width) const;
    int worldToScreenY(double world_y, int screen_height) const;
};

#endif