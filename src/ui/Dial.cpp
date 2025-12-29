#include "ui/Dial.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

Dial::Dial(double minValue, double maxValue, const std::string& label, const std::string& unit)
    : minValue(minValue), maxValue(maxValue), currentValue(minValue), displayValue(minValue),
      label(label), unit(unit), startAngle(-225.0 * M_PI / 180.0), endAngle(45.0 * M_PI / 180.0),
      smoothingFactor(0.15) {
}

Dial::~Dial() {
}

void Dial::setValue(double value) {
    currentValue = std::clamp(value, minValue, maxValue);
    displayValue = lerp(displayValue, currentValue, smoothingFactor);
}

double Dial::lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

double Dial::valueToAngle(double value) {
    double normalized = (value - minValue) / (maxValue - minValue);
    return startAngle + normalized * (endAngle - startAngle);
}

void Dial::drawArc(SDL_Renderer* renderer, int centerX, int centerY, int radius,
                   double startAngle, double endAngle, int segments) {
    for (int i = 0; i < segments; i++) {
        double angle1 = startAngle + (endAngle - startAngle) * i / segments;
        double angle2 = startAngle + (endAngle - startAngle) * (i + 1) / segments;

        int x1 = centerX + static_cast<int>(radius * cos(angle1));
        int y1 = centerY + static_cast<int>(radius * sin(angle1));
        int x2 = centerX + static_cast<int>(radius * cos(angle2));
        int y2 = centerY + static_cast<int>(radius * sin(angle2));

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void Dial::drawTicks(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    double range = maxValue - minValue;
    int majorTickInterval = 1000;
    int minorTickInterval = 250;

    if (range <= 100) {
        majorTickInterval = 20;
        minorTickInterval = 5;
    } else if (range <= 500) {
        majorTickInterval = 100;
        minorTickInterval = 25;
    } else if (range <= 2000) {
        majorTickInterval = 500;
        minorTickInterval = 100;
    }

    for (double value = minValue; value <= maxValue; value += minorTickInterval) {
        double angle = valueToAngle(value);
        bool isMajor = (static_cast<int>(value) % majorTickInterval == 0);

        int tickLength = isMajor ? 15 : 8;
        int tickWidth = isMajor ? 2 : 1;
        int innerRadius = radius - tickLength;

        int x1 = centerX + static_cast<int>(innerRadius * cos(angle));
        int y1 = centerY + static_cast<int>(innerRadius * sin(angle));
        int x2 = centerX + static_cast<int>(radius * cos(angle));
        int y2 = centerY + static_cast<int>(radius * sin(angle));

        SDL_Color tickColor = isMajor ? SDL_Color{220, 220, 220, 255} : SDL_Color{140, 140, 140, 180};
        SDL_SetRenderDrawColor(renderer, tickColor.r, tickColor.g, tickColor.b, tickColor.a);

        for (int w = 0; w < tickWidth; w++) {
            SDL_RenderDrawLine(renderer, x1 + w, y1, x2 + w, y2);
        }
    }

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 100);
    drawArc(renderer, centerX, centerY, radius - 2, startAngle, endAngle, 80);
    drawArc(renderer, centerX, centerY, radius - 25, startAngle, endAngle, 80);
}

void Dial::drawNumbers(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font) {
    if (font == nullptr) return;

    double range = maxValue - minValue;
    int numberInterval = 1000;

    if (range <= 100) {
        numberInterval = 20;
    } else if (range <= 500) {
        numberInterval = 100;
    } else if (range <= 2000) {
        numberInterval = 500;
    }

    for (double value = minValue; value <= maxValue; value += numberInterval) {
        double angle = valueToAngle(value);
        int textRadius = radius - 35;

        int x = centerX + static_cast<int>(textRadius * cos(angle));
        int y = centerY + static_cast<int>(textRadius * sin(angle));

        std::ostringstream oss;
        oss << static_cast<int>(value);
        std::string text = oss.str();

        SDL_Color numberColor = {200, 200, 200, 255};
        drawText(renderer, text, x - 10, y - 8, numberColor, font);
    }
}

void Dial::drawNeedle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    double angle = valueToAngle(displayValue);
    int needleLength = radius - 20;
    int needleTailLength = 20;

    int tipX = centerX + static_cast<int>(needleLength * cos(angle));
    int tipY = centerY + static_cast<int>(needleLength * sin(angle));
    int tailX = centerX - static_cast<int>(needleTailLength * cos(angle));
    int tailY = centerY - static_cast<int>(needleTailLength * sin(angle));

    SDL_SetRenderDrawColor(renderer, 255, 100, 80, 255);
    SDL_RenderDrawLine(renderer, tailX, tailY, tipX, tipY);

    SDL_SetRenderDrawColor(renderer, 255, 140, 120, 150);
    int glowOffsets[] = {-1, 0, 1};
    for (int dx : glowOffsets) {
        for (int dy : glowOffsets) {
            if (dx == 0 && dy == 0) continue;
            SDL_RenderDrawLine(renderer, tailX + dx, tailY + dy, tipX + dx, tipY + dy);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 100, 80, 255);
    for (int r = 0; r < 6; r++) {
        for (int i = 0; i < 360; i += 30) {
            double rad = i * M_PI / 180.0;
            int x = centerX + static_cast<int>(r * cos(rad));
            int y = centerY + static_cast<int>(r * sin(rad));
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    for (int r = 1; r < 4; r++) {
        for (int i = 0; i < 360; i += 20) {
            double rad = i * M_PI / 180.0;
            int x = centerX + static_cast<int>(r * cos(rad));
            int y = centerY + static_cast<int>(r * sin(rad));
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void Dial::drawLabel(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font) {
    if (font == nullptr) return;

    int labelY = centerY + radius / 3;
    SDL_Color labelColor = {160, 160, 160, 255};
    drawText(renderer, label, centerX - static_cast<int>(label.length() * 3), labelY, labelColor, font);
}

void Dial::drawReadout(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font) {
    if (font == nullptr) return;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << displayValue;
    std::string valueText = oss.str();

    int readoutY = centerY + radius / 2;
    SDL_Color readoutColor = {240, 240, 240, 255};

    SDL_Surface* valueSurface = TTF_RenderText_Blended(font, valueText.c_str(), readoutColor);
    if (valueSurface != nullptr) {
        int valueX = centerX - valueSurface->w / 2;
        SDL_Texture* valueTexture = SDL_CreateTextureFromSurface(renderer, valueSurface);
        if (valueTexture != nullptr) {
            SDL_Rect valueRect = {valueX, readoutY, valueSurface->w, valueSurface->h};
            SDL_RenderCopy(renderer, valueTexture, nullptr, &valueRect);
            SDL_DestroyTexture(valueTexture);
        }
        SDL_FreeSurface(valueSurface);
    }

    if (!unit.empty()) {
        int unitY = readoutY + 15;
        SDL_Color unitColor = {140, 140, 140, 255};

        SDL_Surface* unitSurface = TTF_RenderText_Blended(font, unit.c_str(), unitColor);
        if (unitSurface != nullptr) {
            int unitX = centerX - unitSurface->w / 2;
            SDL_Texture* unitTexture = SDL_CreateTextureFromSurface(renderer, unitSurface);
            if (unitTexture != nullptr) {
                SDL_Rect unitRect = {unitX, unitY, unitSurface->w, unitSurface->h};
                SDL_RenderCopy(renderer, unitTexture, nullptr, &unitRect);
                SDL_DestroyTexture(unitTexture);
            }
            SDL_FreeSurface(unitSurface);
        }
    }
}

void Dial::drawText(SDL_Renderer* renderer, const std::string& text, int x, int y,
                    SDL_Color color, TTF_Font* font) {
    if (font == nullptr || text.empty()) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface == nullptr) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstrect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Dial::draw(SDL_Renderer* renderer, int centerX, int centerY, int radius, TTF_Font* font) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    drawTicks(renderer, centerX, centerY, radius);
    drawNumbers(renderer, centerX, centerY, radius, font);
    drawNeedle(renderer, centerX, centerY, radius);
    drawLabel(renderer, centerX, centerY, radius, font);
    drawReadout(renderer, centerX, centerY, radius, font);
}