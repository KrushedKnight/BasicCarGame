#ifndef GROUND_H
#define GROUND_H

#include <SDL_render.h>

class Camera;

class Ground {
public:
    int grid_spacing;
    unsigned char line_r, line_g, line_b;

    Ground(int spacing = 100);

    void draw(SDL_Renderer* renderer, const Camera* camera,
              int screenWidth, int screenHeight);
};

#endif
