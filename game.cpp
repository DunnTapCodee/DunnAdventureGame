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
bool moving_left = false, moving_right = false, jumping = false,
     lastMovingLeft = false, lastMovingRight = false;
Uint32 lastMoveTime = SDL_GetTicks();

MainCharacter* dunn;
MainCharacter* ronaldo;
GameObject* ball;
vector<GameObject*> obj;
bool ball_shot = false; int mouseX, mouseY;

// Hàm xử lý bắn bóng
void ShootingBall(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        ball_shot = true;   

        SDL_GetMouseState(&mouseX, &mouseY);
    }   

    if (ball_shot)
        {
            // Xuất phát từ vị trí của kanie
            GameObject* newBall = new GameObject("media/ball.png", graphics, ronaldo->x + 60 , ronaldo->y + 100);
        
            // Đặt mục tiêu và tăng tốc độ lên 1.2 lần
            newBall->setTarget(mouseX, mouseY, 1.2f);

            obj.push_back(newBall);

            ball_shot = false;
    }
}

// Hàm di chuyển nhân vật
void Moving() {
    clock_t start = clock();
    dunn->animation();
    ronaldo->animation();
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
                    case SDLK_a: moving_left = true;  break;
                    case SDLK_d: moving_right = true; break;
                    case SDLK_SPACE: jumping = true; break;
                    default: break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: moving_left = false; lastMovingLeft = true;  break;
                    case SDLK_d: moving_right = false; lastMovingRight = true;break;
                    case SDLK_SPACE: jumping = false;  break;
                    default: break;
                }
            }
        }
    }

    if (moving_left)  { dunn->move_left(); ronaldo->move_left(); }
    if (moving_right) { dunn->move_right(); ronaldo->move_right(); }
    if (jumping && dunn->is_touching_ground()) {
        dunn->jump(JUMP_SPEED);
    }

    // if (SDL_GetTicks() - lastMoveTime > 200) {
    //     if (moving_left) ronaldo->move_left();
    //     if (moving_right) ronaldo->move_right();
    //     if (jumping && ronaldo->is_touching_ground()) ronaldo->jump(JUMP_SPEED);
    //     lastMoveTime = SDL_GetTicks();
    // }

    int used_time = clock() - start;
    SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
}


// Cập nhật vị trí bóng
// Xóa bóng nếu reacedTarget
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
    dunn-> loadFrameLeft( { "media/dunnleft.png", "media/dunn2left.png", "media/dunn3left.png", "media/dunn2left.png", "media/dunn2jump.png"} );
    dunn-> loadFrameRight( {"media/dunn.png", "media/dunn2.png", "media/dunn3.png", "media/dunn2.png", "media/dunnjump.png"} );

    ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
    ball = new GameObject("media/ball.png", graphics, 150, GROUND_LEVEL);

    int width = 50;
    int frames = 0;
    int distance_moved = 0; 
    const int FRAME_CHANGE_DISTANCE = 40; 

    while (running) {
        Moving();
        UpdateBalls();
        ronaldo -> y = GROUND_LEVEL + 30;
        if (moving_left || moving_right) {
            distance_moved += MOVE_SPEED; 
            if (distance_moved >= FRAME_CHANGE_DISTANCE) {
                frames = (frames + 1) % 4; 
                distance_moved = 0; // Reset khoảng cách
            }
        }
    
        if (moving_left &&  dunn->is_touching_ground())
           {
                width = 50;
                dunn ->texture = dunn -> framesLeft[frames];
           }
        else if (moving_right && dunn->is_touching_ground())
            {
                width = 50;
                dunn ->texture = dunn -> framesRight[frames];
            }
        else if (moving_right && !dunn->is_touching_ground() ) { dunn ->texture = dunn -> framesRight[4]; width = 100;}
        else if (moving_left  && !dunn->is_touching_ground() ) {dunn ->texture = dunn -> framesLeft[4];width = 100; }
        else if ( dunn -> is_touching_ground() )
            {
                width = 50;
                if (lastMovingLeft == true) { dunn ->texture = dunn -> framesLeft[0];}
                else if (lastMovingRight == true) { dunn ->texture = dunn -> framesRight[0]; }
                lastMovingLeft = false;
                lastMovingRight = false;
            }

        graphics.render(dunn->texture, dunn->x, dunn->y, width, ronaldo->texture, ronaldo->x, ronaldo->y);
        for (auto& ball : obj) {
            graphics.renderTexture(ball->texture, ball->x, ball->y, 20 , 20);
        }
        graphics.presentScene();  
    }

    

    graphics.quit();
    return 0;
}

