#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"
#include "MainCharacter.h"
#include "graphics.h"



using namespace std;

void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }   
}

int main(int argc, char* argv[]) {
    Graphics graphics;

    // tạo nhân vật chính
    
    graphics.init();
    
    SDL_Texture* background = graphics.loadTexture("media/background.jpg", graphics.renderer);
    graphics.background = background;

    MainCharacter* dunn = new MainCharacter("media/dunn.png", graphics, 100, GROUND_LEVEL);
    MainCharacter* kanie = new MainCharacter("media/kanie.png",graphics, 150, GROUND_LEVEL);
    bool running = true;
    SDL_Event event;
    bool moving_left = 0, moving_right = 0, jumping = 0;
    Uint32 lastMoveTime = SDL_GetTicks(); 

    while (running) 
       {
            clock_t start = clock();
            dunn ->animation();
            kanie ->animation();
            graphics.render(dunn->texture, dunn->x, dunn->y, kanie ->texture, kanie ->x, kanie ->y);
    
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || 
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN))
                     {
                            running = false;
                     }    
                else
                     {
                        if (event.type == SDL_KEYDOWN) {
                            switch (event.key.keysym.sym) {
                                case SDLK_a: { moving_left = 1; break; }
                                case SDLK_d: { moving_right = 1; break; }
                                case SDLK_SPACE: { jumping = 1; break; }
                                default: break;
                            }
                        }
                        else if (event.type == SDL_KEYUP) {
                            switch (event.key.keysym.sym) {
                                case SDLK_a: { moving_left = 0; break; }
                                case SDLK_d: { moving_right = 0; break; }
                                case SDLK_SPACE: { jumping = 0 ;break; }
                                default: break;
                            }
                        }
                     }
            }

        if (moving_left)
           {
                 dunn ->move_left();
                 kanie ->move_left();
           }
        if (moving_right)
           {
                 dunn ->move_right();
                 kanie ->move_right();
           }
        if (jumping)
           {
            // nếu chạm đất mới cho nhảy
            if (dunn ->is_touching_ground() == true)
               {
                     dunn ->jump(JUMP_SPEED);
                     kanie ->jump(JUMP_SPEED);
               }
            }
            
        if (SDL_GetTicks() - lastMoveTime > 200)
            {
                if (moving_left) kanie->move_left();
                if (moving_right) kanie->move_right();
                if (jumping && kanie->is_touching_ground()) kanie->jump(JUMP_SPEED);
                lastMoveTime = SDL_GetTicks();  // Reset bộ đếm
            }
    

        // cái này để delay đúng 16ms
        int used_time = clock() - start;
        SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
        }
    graphics.quit();

    return 0;
    }