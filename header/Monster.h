#ifndef MONSTER_H
#define MONSTER_H

#include <SDL2/SDL.h>
#include "graphics.h"

#include <vector>
#include <cstdlib>
#include <ctime>



class Monster {
    public:
        int x, y;
        int targetX;
        int speedX
        bool reachedTarget = false;
        SDL_Texture* texture;

    
        Monster(const char* filePath, Graphics& graphics, float startX, float startY)
            : x(startX), y(startY), targetX(startX), targetY(startY), speedX(10)
        {
            texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
        ~Monster() {
            SDL_DestroyTexture(texture);
        }
    
        // sau khi nhận được vị ví mouse_event, set up đường đi, speed cho balls
        void setTarget( int current_X, int targetX)
            {
                std::srand(std::time(nullptr)); // Đặt seed theo thời gian hiện tại
                int randomNumber = std::rand() % 100;
                targetX = randomNumber;
            }
    
        // cập nhật trạng thái của ball đã tới vị trí target chưa
        void update() {
            if (!reachedTarget) {
                x += speedX;
                if (abs(x - targetX) < 10) {  reachedTarget = true; }

            }
        }
    
};

#endif

