#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include <SDL2/SDL.h>
#include "graphics.h"

const int JUMP_STRENGTH = -4;
const int GRAVITY = 1;
constexpr int GROUND_LEVEL = 400;

class MainCharacter 
{
    public:
    int x, y;
    int current_speed = 0;
    bool is_jumping = false;

    SDL_Texture* texture;

    MainCharacter (const char* filePath, Graphics& graphics, int startX, int startY)
     : x(startX), y(startY), is_jumping (false)
        {
                texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
    ~MainCharacter()
      {
            SDL_DestroyTexture(texture);
      }

   void jump(int speed) {
      if (is_touching_ground()) { // Chỉ cho phép nhảy khi đang đứng trên mặt đất
          current_speed = speed;
          is_jumping = true;
      }
  }
  
  void animation() {
      if (is_jumping) {
          y -= current_speed;
          current_speed -= GRAVITATIONAL_ACCELERATION; // Trừ lực hấp dẫn dần
          if (current_speed <= 0) {
              is_jumping = false; // Khi tốc độ nhảy hết, rơi xuống
          }
      } else {
          int next_y = y - current_speed;
          if (next_y + 10 >= GROUND_LEVEL) { // Chạm đất
              y = GROUND_LEVEL - 10;
              current_speed = 0;
          } else {
              current_speed -= GRAVITATIONAL_ACCELERATION;
              y = next_y;
          }
      }
  }
  
  bool is_touching_ground() {
      return (y + 10 >= GROUND_LEVEL);
  }
  
   void move_left()
     {
        x = x - MOVE_SPEED;
     }
   void move_right()
     {
         x = x + MOVE_SPEED;
     }

   void render(Graphics& graphics) {
      graphics.renderTexture(texture, x, y, 120, 120);
  }
    
};

#endif

