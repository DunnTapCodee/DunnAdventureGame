#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include "graphics.h"
#include <iostream>
#include <cmath>
#include <vector>



class GameObject {
    public:
        float x, y;
        float targetX, targetY;
        float speedX, speedY;
        bool reachedTarget = false;
        SDL_Texture* texture;

    
        GameObject(const char* filePath, Graphics& graphics, float startX, float startY)
            : x(startX), y(startY), targetX(startX), targetY(startY), speedX(0), speedY(0)
        {
            texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
        ~GameObject() {
            SDL_DestroyTexture(texture);
        }
    
        void setTarget(float tx, float ty, float speedMultiplier = 1.0f) {
            targetX = tx;
            targetY = ty;
            float dx = targetX - x;
            float dy = targetY - y;
            float length = sqrt(dx * dx + dy * dy);
            speedX = (dx / length) * 5.0f * speedMultiplier;
            speedY = (dy / length) * 5.0f * speedMultiplier;
        }
    
        void update() {
            if (!reachedTarget) {
                x += speedX;
                y += speedY;
    
                if (fabs(x - targetX) < 5 && fabs(y - targetY) < 5) {
                    reachedTarget = true;
                }
            }
        }
    
};

#endif
