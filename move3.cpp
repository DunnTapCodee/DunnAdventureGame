#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"

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
    Game game;
    
    // Tạo nhân vật dunn với tọa độ mặt đất là 300
    const int GROUND_LEVEL = 300;
    MainCharacter* dunn = new MainCharacter("media/dunn.png", game.graphics, 100, GROUND_LEVEL);
    game.addMainCharacter(dunn);
    //game.addObject();

    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)) {
                running = false;
            } else {
                game.update(event); // Cập nhật vị trí vật thể
            }
        }

        game.render(); // Vẽ toàn bộ game
        SDL_Delay(4); // Giảm thời gian delay để chuyển động mượt mà hơn
    }
    
    return 0;
}
