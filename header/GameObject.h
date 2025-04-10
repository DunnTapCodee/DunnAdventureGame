#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include "graphics.h"
#include <iostream>
#include <cmath>
#include <vector>



class GameObject {
    public:
        int through = THROUGH;
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
    
        // sau khi nhận được vị ví mouse_event, set up đường đi, speed cho balls
        void setTarget(float tx, float ty, float speedMultiplier = 1.2f) {
            targetX = tx - 10;
            targetY = ty - 10;
            float dx = targetX - x ;
            float dy = targetY - y ;
            float length = sqrt(dx * dx + dy * dy);
            speedX = (dx / length) * 5.0f * speedMultiplier;
            speedY = (dy / length) * 5.0f * speedMultiplier;
        }
    
        // cập nhật trạng thái của ball đã tới vị trí target chưa
        void update() {
            if (!reachedTarget) {
                x += speedX;
                y += speedY;
    
                if (fabs(x - targetX) < 3 && fabs(y - targetY) < 3) {
                    reachedTarget = true;
                }

            }
        }
    
};

#endif

