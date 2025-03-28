#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graphics.h"
#include <vector>

const int JUMP_STRENGTH = -4;
const int GRAVITY = 1;
constexpr int GROUND_LEVEL = 400;

class MainCharacter 
{
    public:
    int x, y;
    int current_speed = 0;
    bool is_jumping = false;
    bool overMapLeft = false, overMapRight = false;
    SDL_Texture* texture;
    Graphics& graphicsRef;

    std::vector < SDL_Texture* > framesRight;
    std::vector < SDL_Texture* > framesLeft;
    std::vector < SDL_Texture* > Maps;

    MainCharacter (const char* filePath, Graphics& graphics, int startX, int startY)
     : x(startX), y(startY), is_jumping (false),  graphicsRef(graphics)
        {
                texture = graphics.loadTexture(filePath, graphics.renderer);
        }
    
    void loadFrameLeft( std::vector <const char*> filePaths)
        {
              for (const char* path : filePaths)
                 {
                     SDL_Texture* texture = graphicsRef.loadTexture( path, graphicsRef.renderer);
                     if (texture) framesLeft.push_back(texture);
                 }
        }
     
    void loadFrameRight( std::vector <const char*> filePaths)
        {
              for (const char* path : filePaths)
                 {
                     SDL_Texture* texture = graphicsRef.loadTexture( path, graphicsRef.renderer);
                     if (texture) framesRight.push_back(texture);
                 }
        }

    void loadMaps( std::vector <const char*> filePaths)
        {
              for (const char* path : filePaths)
                 {
                     SDL_Texture* texture = graphicsRef.loadTexture( path, graphicsRef.renderer);
                     if (texture) Maps.push_back(texture);
                 }
        }

    ~MainCharacter()
      {
            SDL_DestroyTexture(texture);
            for (SDL_Texture* text : framesRight) SDL_DestroyTexture(text);
            for (SDL_Texture* text : framesLeft) SDL_DestroyTexture(text);
      }
    // vector các frame của maincharacter
    
      
   // hàm nhảy, với vận tốc ban đầu = JUMP_SPEED 28, biến is_jumping = true
   void jump(int speed) {
      if (is_touching_ground()) { // Chỉ cho phép nhảy khi đang đứng trên mặt đất
          current_speed = speed;
          is_jumping = true;
      }
  }
  
   // 
   void animation() {
      if (is_jumping) {
          y -= current_speed;
          current_speed -= GRAVITATIONAL_ACCELERATION; // Trừ lực hấp dẫn dần -> tốc độ nhảy giảm dần
          if (current_speed <= 0) {
              is_jumping = false; // Khi tốc độ nhảy hết, rơi xuống
          }
      } else {
          // khi vào đến lệnh else này tức nhân vật đang rơi xuống
          int next_y = y - current_speed;
          if (next_y + 10 >= GROUND_LEVEL) { // Chạm đất
              y = GROUND_LEVEL - 10;
              current_speed = 0;
          } else {
              current_speed -= GRAVITATIONAL_ACCELERATION;
               // lúc này current_speed mang giá trị âm -> next_y sẽ tăng dần
              y = next_y;
          }
      }
  }
  
   bool is_touching_ground() {
      return (y + 10 >= GROUND_LEVEL); 
      // khi nhân vật ở độ cao hơn mặt đật chỉ 10 đơn vị thì mới coi là chạm đất
  }
  
   void move_left()
     {
        x = x - MOVE_SPEED;
        if (x <= 50) {
            overMapLeft = true;
        }
     }
      
   void move_right()
     {
         x = x + MOVE_SPEED;
         if (x >= SCREEN_WIDTH - 50)
            {
                overMapRight = true;
            }
     }



    
//    void render(Graphics& graphics) {
//       graphics.renderTexture(texture, x, y, 120, 120);
//   }
    
};

#endif

