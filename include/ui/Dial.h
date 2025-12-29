#ifndef DIAL_H
#define DIAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Dial {
public:
    Dial(double minValue, double maxValue, const std::string& label, const std::string& unit);
    ~Dial();

    void setValue(double value);
    void draw(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font = nullptr);

private:
    double minValue;
    double maxValue;
    double currentValue;
    double displayValue;
    std::string label;
    std::string unit;

    double startAngle;
    double endAngle;
    double smoothingFactor;

    void drawArc(SDL_Renderer* renderer, int centerX, int centerY, int radius,
                 double startAngle, double endAngle, int segments = 64);
    void drawTicks(SDL_Renderer* renderer, int centerX, int centerY, int radius);
    void drawNumbers(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font);
    void drawNeedle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
    void drawLabel(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font);
    void drawReadout(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font);

    void drawText(SDL_Renderer* renderer, const std::string& text, int x, int y,
                  SDL_Color color, TTF_Font* font);
    double valueToAngle(double value);
    double lerp(double a, double b, double t);
};

#endif