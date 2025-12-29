#ifndef FREEBODYDIAGRAM_H
#define FREEBODYDIAGRAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include "vehicle/Car.h"

class FreeBodyDiagram {
public:
    FreeBodyDiagram();
    ~FreeBodyDiagram();

    bool initialize(const char* fontPath = nullptr, int fontSize = 16);

    void draw(SDL_Renderer* renderer, const Car& car);

    void toggleVisibility();
    bool isVisible() const { return visible; }

private:
    TTF_Font* font;
    bool visible;
    int fontSize;

    void drawPanel(SDL_Renderer* renderer, int windowWidth, int windowHeight);
    void drawCarRepresentation(SDL_Renderer* renderer, int centerX, int centerY, double angle);
    void drawForceVector(SDL_Renderer* renderer, int centerX, int centerY, Eigen::Vector2d force, SDL_Color color, const std::string& label, double scale);
    void drawArrowHead(SDL_Renderer* renderer, int tipX, int tipY, double angle);
    void drawText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);

    SDL_Color getForceColor(const std::string& forceName);
};

#endif
