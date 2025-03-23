#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"
#include "MainCharacter.h"
#include "graphics.h"
#include <vector>
#include <cmath>

using namespace std;

Graphics graphics;
SDL_Event event;
bool running = true;
bool moving_left = false, moving_right = false, jumping = false;
Uint32 lastMoveTime = SDL_GetTicks();

MainCharacter* dunn;
MainCharacter* kanie;
GameObject* ball;
vector<GameObject*> obj;


// Hàm xử lý bắn bóng
void ShootingBall(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Xuất phát từ vị trí của kanie
        GameObject* newBall = new GameObject("media/kanie.png", graphics, kanie->x + 50, kanie->y + 20);
        
        // Đặt mục tiêu và tăng tốc độ lên 1.2 lần
        newBall->setTarget(mouseX, mouseY, 1.2f);

        obj.push_back(newBall);
    }
}

// Hàm di chuyển nhân vật
void Moving() {
    clock_t start = clock();
    dunn->animation();
    kanie->animation();
    // graphics.render(dunn->texture, dunn->x, dunn->y, kanie->texture, kanie->x, kanie->y);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || 
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)) {
            running = false;
        } else {
            ShootingBall(event);  // Xử lý bắn bóng khi chuột được nhấn
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: moving_left = true; break;
                    case SDLK_d: moving_right = true; break;
                    case SDLK_SPACE: jumping = true; break;
                    default: break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: moving_left = false; break;
                    case SDLK_d: moving_right = false; break;
                    case SDLK_SPACE: jumping = false; break;
                    default: break;
                }
            }
        }
    }

    if (moving_left)  { dunn->move_left(); kanie->move_left(); }
    if (moving_right) { dunn->move_right(); kanie->move_right(); }
    if (jumping && dunn->is_touching_ground()) {
        dunn->jump(JUMP_SPEED);
        kanie->jump(JUMP_SPEED);
    }

    if (SDL_GetTicks() - lastMoveTime > 200) {
        if (moving_left) kanie->move_left();
        if (moving_right) kanie->move_right();
        if (jumping && kanie->is_touching_ground()) kanie->jump(JUMP_SPEED);
        lastMoveTime = SDL_GetTicks();
    }

    int used_time = clock() - start;
    SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
}


// Cập nhật vị trí bóng
void UpdateBalls() {
    for (auto it = obj.begin(); it != obj.end();) {
        (*it)->update();

        if ((*it)->reachedTarget) {
            delete *it;
            it = obj.erase(it);
        } else {
            ++it;
        }
    }
}

// Main function
int main(int argc, char* argv[]) {
    graphics.init();

    SDL_Texture* background = graphics.loadTexture("media/background.jpg", graphics.renderer);
    graphics.background = background;

    dunn = new MainCharacter("media/dunn.png", graphics, 100, GROUND_LEVEL);
    kanie = new MainCharacter("media/kanie.png", graphics, 150, GROUND_LEVEL);
    ball = new GameObject("media/kanie.png", graphics, 150, GROUND_LEVEL);

    while (running) {
        Moving();
        UpdateBalls();

        graphics.prepareScene(graphics.background);
        graphics.render(dunn->texture, dunn->x, dunn->y, kanie->texture, kanie->x, kanie->y);
        for (auto& ball : obj) {
            graphics.renderTexture(ball->texture, ball->x, ball->y, 80, 80);
        }
        graphics.presentScene();
        
        
    }

    graphics.quit();
    return 0;
}
