#ifndef MONSTER_H
#define MONSTER_H

#include <SDL2/SDL.h>
#include "graphics.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>



class Monster {
    public:
        int x, y;
        int targetX;
        int speedX = 2;
        bool reachedTarget = false;
        SDL_Texture* texture;

    
        Monster(const char* filePath, Graphics& graphics, float startX, float startY)
            : x(startX), y(startY)
        {
            texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
        ~Monster() {
            SDL_DestroyTexture(texture);
        }
    
        // sau khi nhận được vị ví mouse_event, set up đường đi, speed cho balls
        void setTarget( int TargetX)
            {
                targetX = TargetX;
                reachedTarget = false;
            }

        // cập nhật trạng thái của ball đã tới vị trí target chưa
        void update() {
            if (!reachedTarget) {
                int distance = abs(x - targetX);
                int moveStep = std::min(speedX, distance); 
        
                if (x < targetX) x += moveStep;
                else if (x > targetX) x -= moveStep;
        
        
                if (distance < 10) {  
                    reachedTarget = true;
                    
                }
            }
        }
    
};

#endif

