#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include "graphics.h"
#include <iostream>


class GameObject {
public:
    int x, y;
    int velocityX;
    int velocityY;
    bool isJumping;
    SDL_Texture* texture;

    GameObject(const char* filePath, Graphics& graphics, int startX, int startY)
        : x(startX), y(startY), velocityX(0), velocityY(0),isJumping(false) {
        texture = graphics.loadTexture(filePath, graphics.renderer);
    }

    ~GameObject() {
        SDL_DestroyTexture(texture);
    }

    void render(Graphics& graphics) {
        graphics.renderTexture(texture, x, y, graphics.renderer, 0.1f);
    }
};

#endif
