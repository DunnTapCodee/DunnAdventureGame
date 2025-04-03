#ifndef MONSBALL_H
#define MONSBALL_H

#include <SDL2/SDL.h>
#include "graphics.h"
#include <iostream>
#include <cmath>
#include <vector>



class MonsBall {
    public:
        int x, y;
        int target;
        int speed;
        bool reachedTarget = false;

        SDL_Texture* texture;

    
        MonsBall(const char* filePath, Graphics& graphics, int startX, int startY)
            : x(startX), y(startY), speed(3)
        {
            texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
        ~MonsBall() {
            SDL_DestroyTexture(texture);
        }
    
        // cập nhật trạng thái của ball đã tới vị trí target chưa
        void update() {
            if (!reachedTarget) {
                y += speed;
                if ( y >= 600) reachedTarget = true;
            }
        }
    
};

#endif

