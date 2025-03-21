#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include <SDL2/SDL.h>
#include "graphics.h"

const int JUMP_STRENGTH = -4;
const int GRAVIRY = 1;
const int GROUND_LEVEL = 300;

class MainCharacter 
{
    public:
    int x, y;
    int velocityX, velocityY;
    bool isJumping;

    SDL_Texture* texture;

    MainCharacter (const char* filePath, Graphics& graphics, int startX, int startY)
     : x(startX), y(startY), velocityX(0), velocityY(0), isJumping (false)
        {
                texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
    ~MainCharacter()
      {
            SDL_DestroyTexture(texture);
      }

      void moveLeft() { velocityX = -SPEED; }
      void moveRight() { velocityX = SPEED; }
      void stop() { velocityX = 0; }

      void jump(bool moveleft, bool moveright)
        {
            if ( isJumping == false)
               {
                 velocityY = JUMP_STRENGTH;
                 isJumping = true;
                if (moveleft == true)
                   {
                      velocityX = -2;
                   }
                else if (moveright == true )
                   {
                      velocityX = 2;
                   }
               }

        }

      void applyPhysics()
        {
            y = y + velocityY;
            x = x + velocityX;
            velocityY = velocityY + GRAVIRY;

            if ( y == GROUND_LEVEL)
               {
                    isJumping = false;
               } 
        }
        
    void render(Graphics& graphics)
        {
            graphics.renderTexture(texture, x, y, graphics.renderer, 0.1f);
        }
};

#endif

