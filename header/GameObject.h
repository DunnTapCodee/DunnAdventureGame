#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include "graphics.h"

class GameObject {
public:
    int x, y;
    int velocity;
    SDL_Texture* texture;

    GameObject(const char* filePath, Graphics& graphics, int startX, int startY, int vel)
        : x(startX), y(startY), velocity(vel) {
        texture = graphics.loadTexture(filePath, graphics.renderer);
    }

    ~GameObject() {
        SDL_DestroyTexture(texture);
    }

    void move(int dx, int dy) {
        x += dx * velocity;
        y += dy * velocity;
    }

    void render(Graphics& graphics) {
        graphics.renderTexture(texture, x, y, graphics.renderer, 0.1f);
    }
};

#endif
