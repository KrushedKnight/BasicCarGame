#include "ui/FreeBodyDiagram.h"
#include "config/RenderingConstants.h"
#include <cmath>

FreeBodyDiagram::FreeBodyDiagram() : font(nullptr), visible(true), fontSize(16) {
}

FreeBodyDiagram::~FreeBodyDiagram() {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
}

bool FreeBodyDiagram::initialize(const char* fontPath, int fontSize) {
    this->fontSize = fontSize;

    if (fontPath != nullptr) {
        font = TTF_OpenFont(fontPath, fontSize);
    }

    if (font == nullptr) {
        const char* fallbackFonts[] = {
            "/System/Library/Fonts/Menlo.ttc",
            "/System/Library/Fonts/Courier.dfont",
            "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
            "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
            "C:\\Windows\\Fonts\\consola.ttf",
            nullptr
        };

        for (int i = 0; fallbackFonts[i] != nullptr && font == nullptr; i++) {
            font = TTF_OpenFont(fallbackFonts[i], fontSize);
        }
    }

    if (font == nullptr) {
        SDL_Log("Failed to load font for FreeBodyDiagram: %s", TTF_GetError());
        return false;
    }

    return true;
}

void FreeBodyDiagram::toggleVisibility() {
    visible = !visible;
}

void FreeBodyDiagram::drawPanel(SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    const int panelSize = 300;
    const int margin = 50;

    SDL_Rect panel = {windowWidth - panelSize - margin, windowHeight - panelSize - margin, panelSize, panelSize};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &panel);
}

void FreeBodyDiagram::drawCarRepresentation(SDL_Renderer* renderer, int centerX, int centerY, double angle) {
    const int carWidth = 20;
    const int carLength = 30;

    SDL_Point carPoints[5];
    double halfWidth = carWidth / 2.0;
    double halfLength = carLength / 2.0;

    double corners[4][2] = {
        {-halfWidth, -halfLength},
        {halfWidth, -halfLength},
        {halfWidth, halfLength},
        {-halfWidth, halfLength}
    };

    for (int i = 0; i < 4; i++) {
        double x = corners[i][0];
        double y = corners[i][1];

        double rotatedX = x * cos(angle) - y * sin(angle);
        double rotatedY = x * sin(angle) + y * cos(angle);

        carPoints[i].x = centerX + static_cast<int>(rotatedX);
        carPoints[i].y = centerY + static_cast<int>(rotatedY);
    }
    carPoints[4] = carPoints[0];

    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawLine(renderer, carPoints[i].x, carPoints[i].y, carPoints[i + 1].x, carPoints[i + 1].y);
    }

    double frontX = halfLength * cos(angle);
    double frontY = halfLength * sin(angle);
    SDL_RenderDrawLine(renderer, centerX, centerY, centerX + frontX, centerY + frontY);
}

void FreeBodyDiagram::drawArrowHead(SDL_Renderer* renderer, int tipX, int tipY, double angle) {
    const int headLength = 8;
    const double headAngle = M_PI / 6;

    int left1X = tipX - headLength * cos(angle - headAngle);
    int left1Y = tipY - headLength * sin(angle - headAngle);
    int left2X = tipX - headLength * cos(angle + headAngle);
    int left2Y = tipY - headLength * sin(angle + headAngle);

    SDL_RenderDrawLine(renderer, tipX, tipY, left1X, left1Y);
    SDL_RenderDrawLine(renderer, tipX, tipY, left2X, left2Y);
}

void FreeBodyDiagram::drawText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    if (font == nullptr || text.empty()) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface == nullptr) {
        return;
    }

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

SDL_Color FreeBodyDiagram::getForceColor(const std::string& forceName) {
    if (forceName == "FL Friction") {
        return {255, 100, 100, 255};
    } else if (forceName == "FR Friction") {
        return {100, 255, 100, 255};
    } else if (forceName == "RL Friction") {
        return {100, 100, 255, 255};
    } else if (forceName == "RR Friction") {
        return {255, 165, 0, 255};
    }
    return {255, 255, 255, 255};
}

void FreeBodyDiagram::drawForceVector(SDL_Renderer* renderer, int centerX, int centerY, Eigen::Vector2d force, SDL_Color color, const std::string& label, double scale) {
    if (force.norm() < 0.1) return;

    int endX = centerX + static_cast<int>(force.x() * scale);
    int endY = centerY - static_cast<int>(force.y() * scale);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, centerX, centerY, endX, endY);

    double angle = atan2(-force.y(), force.x());
    drawArrowHead(renderer, endX, endY, angle);

    if (!label.empty()) {
        drawText(renderer, label, endX + 5, endY - 10, color);
    }
}

void FreeBodyDiagram::draw(SDL_Renderer* renderer, const Car& car) {
    if (!visible) return;

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    drawPanel(renderer, windowWidth, windowHeight);

    const int panelSize = 300;
    const int margin = 50;
    int centerX = windowWidth - margin - panelSize / 2;
    int centerY = windowHeight - margin - panelSize / 2;

    drawCarRepresentation(renderer, centerX, centerY, car.angular_position);

    double maxMagnitude = 0.0;

    const auto& namedForces = car.getNamedForces();
    for (const auto& pair : namedForces) {
        double mag = pair.second.norm();
        if (mag > maxMagnitude) maxMagnitude = mag;
    }

    double velocityMag = car.velocity.norm();
    double accelMag = car.acceleration.norm();
    if (velocityMag > maxMagnitude) maxMagnitude = velocityMag;
    if (accelMag > maxMagnitude) maxMagnitude = accelMag;

    const double maxDrawLength = (panelSize / 2.0) * 0.85;
    double scale = 1.0;
    if (maxMagnitude > 0.1) {
        scale = maxDrawLength / maxMagnitude;
    }

    for (const auto& pair : namedForces) {
        const std::string& name = pair.first;
        const Eigen::Vector2d& force = pair.second;
        SDL_Color color = getForceColor(name);
        drawForceVector(renderer, centerX, centerY, force, color, name, scale);
    }

    SDL_Color velocityColor = {0, 255, 255, 255};
    drawForceVector(renderer, centerX, centerY, car.velocity, velocityColor, "Velocity", scale);

    SDL_Color accelColor = {255, 255, 0, 255};
    drawForceVector(renderer, centerX, centerY, car.acceleration, accelColor, "Accel", scale);
}
