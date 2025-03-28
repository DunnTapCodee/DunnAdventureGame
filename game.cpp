#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameObject.h"
#include "MainCharacter.h"
#include "graphics.h"
#include "Monster.h"

#include <vector>
#include <cmath>

using namespace std;

Graphics graphics;
SDL_Event event;
SDL_Texture* background;
bool running = true;
bool moving_left = false, moving_right = false, jumping = false,
     lastMovingLeft = false, lastMovingRight = false;
Uint32 lastMoveTime = SDL_GetTicks();

    MainCharacter* dunn;
    MainCharacter* ronaldo;
    GameObject* ball;

    vector<GameObject*> obj;
    vector<Monster* > mons;

    template <typename T>

void freeVector(vector<T*>& vec) {
    for (auto& ptr : vec) {
        SDL_DestroyTexture(ptr->texture);
        ptr->texture = nullptr;
        delete ptr;
    }
    vec.clear();
}

void cleanUp(MainCharacter* dunn, MainCharacter* ronaldo, GameObject* ball, Monster* bat ) {
    freeVector(obj);
    freeVector(mons);

    for (auto& texture : dunn->framesLeft) SDL_DestroyTexture(texture);
    dunn->framesLeft.clear();

    for (auto& texture : dunn->framesRight) SDL_DestroyTexture(texture);
    dunn->framesRight.clear();


    for (auto& texture : bat->frames) SDL_DestroyTexture(texture);
    bat->frames.clear();

    delete dunn; dunn = nullptr;
    delete ronaldo; ronaldo = nullptr;
    delete ball; ball = nullptr;

}


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

    int used_time = clock() - start;
    SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
}

// Cập nhật vị trí bóng
// Xóa bóng nếu reachedTarget
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

//Hàm quản lý frames
void HandleCharacterMovement(MainCharacter* dunn, bool moving_left, bool moving_right, bool &lastMovingLeft, bool &lastMovingRight, int& width)
{
    ronaldo->y = GROUND_LEVEL + 30;

    static int frames = 0;
    static int distance_moved = 0;
    const int FRAME_CHANGE_DISTANCE = 40;

    if (moving_left || moving_right) {
        distance_moved += MOVE_SPEED;
        if (distance_moved >= FRAME_CHANGE_DISTANCE) {
            frames = (frames + 1) % 4;
            distance_moved = 0;
        }
    }


    if (moving_left && dunn->is_touching_ground()) { dunn->texture = dunn->framesLeft[frames]; width = 50;}
    else if (moving_right && dunn->is_touching_ground()) { dunn->texture = dunn->framesRight[frames]; width = 50;} 
    else if (moving_right && !dunn->is_touching_ground()) { dunn->texture = dunn->framesRight[4]; width = 100; }
    else if (moving_left && !dunn->is_touching_ground()) { dunn->texture = dunn->framesLeft[4]; width = 100;}
    else if (dunn->is_touching_ground())
       {
            width = 50;
            if (lastMovingLeft) { dunn->texture = dunn->framesLeft[0];}
            else if (lastMovingRight) { dunn->texture = dunn->framesRight[0]; }
            lastMovingLeft = false;
            lastMovingRight = false;
       }   
    graphics.render(dunn->texture, dunn->x, dunn->y, width, ronaldo->texture, ronaldo->x, ronaldo->y);
    
}


void FirstMonster()
   {
        int randomX = rand() % SCREEN_WIDTH - 100;
        int randomY = rand() % 200 + 50;
        int TargetX = rand() % SCREEN_WIDTH;
        Monster* bat = new Monster("media/obj1.png", graphics, randomX , randomY);
        mons.push_back(bat);
        bat -> setTarget(TargetX);
   }

void UpdateMonster()
   {
    for (auto it = mons.begin(); it != mons.end();)
    {
        (*it)->update();
        if ((*it)->reachedTarget == true)
        {
            int TargetX = rand() % SCREEN_WIDTH;
            // printf("Monster reached target! New target: %d\n", TargetX);
            (*it)->setTarget(TargetX);
        }
        
        bool erased = false;
        if ( abs( dunn->x - (*it)->x ) <= 40  && abs( dunn->y - (*it)->y ) <= 40 )
           {
                delete *it;
                it = mons.erase(it);
                erased = true;
                continue;   
           }
        for (auto itr = obj.begin(); itr != obj.end();)
        {
            if ( abs((*itr)->x - (*it)->x ) <= 20  && abs((*itr)->y - (*it)->y ) <= 20 )
            {
                delete *itr;
                itr = obj.erase(itr);
    
                delete *it;
                it = mons.erase(it);
                erased = true;
                break;
            }
            else
            {
                ++itr;
            }
        }
    
        if (!erased) ++it;
    }
    
   }


void changeMap( MainCharacter* dunn, MainCharacter* ronaldo, bool &change )
   {
        if ( dunn->overMapLeft ) 
           {
                dunn -> x = SCREEN_WIDTH - 50 ;
                ronaldo -> x = SCREEN_WIDTH + 50;
                dunn -> overMapLeft = false;
                change = true;

           }
        else if ( dunn->overMapRight ) 
            {
                dunn -> x = 0;
                ronaldo -> x = 100;
                dunn -> overMapRight = false;
                change = true;
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
    dunn ->loadMaps( {"media/background.jpg","media/background2.jpg", "media/background3.jpg", "media/background4.jpg", "media/background5.jpg", "media/background6.jpg", "media/background7.jpg"});
    
    Monster* bat = new Monster("media/obj1.png", graphics, 100, 100);
    bat ->loadFrame( { "media/obj1.png", "media/obj2.png", "media/obj3.png" } );
    printf("Monster frames loaded: %lu\n", bat->frames.size());

    ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
    ball = new GameObject("media/ball.png", graphics, 150, GROUND_LEVEL);
    int framesMons = 0, distance = 0;
    int FRAME_CHANGE_DISTANCE_MONS = 40;

    int width = 50;
    while (running) {
        Moving(); bool change = false;
        changeMap( dunn, ronaldo, change);
        if (change == true)
            {
                // if (graphics.background) {
                //     SDL_DestroyTexture(graphics.background);
                //     graphics.background = nullptr;
                // }
                int map = ( rand() % 7 );
                graphics.background = dunn ->Maps[map];
                freeVector(obj); freeVector(mons);
                graphics.prepareScene(graphics.background);
            }
        
            UpdateBalls();
            while ( mons.size() <= 3) {
                FirstMonster();
            }


        UpdateMonster();

        HandleCharacterMovement(dunn, moving_left, moving_right, lastMovingLeft, lastMovingRight, width);
        

        for (auto& mon : mons)
        {
            distance += 2;
            if (distance >= FRAME_CHANGE_DISTANCE_MONS) { framesMons = (framesMons + 1) % 3; distance = 0; }
        
            // Kiểm tra tránh crash
            if (!bat->frames.size()) { mon->texture = bat->frames[framesMons]; }
            graphics.renderTexture(bat ->frames[framesMons], mon->x, mon->y, 40, 40);
        }

        for (auto& ball : obj) {
            graphics.renderTexture(ball->texture, ball->x, ball->y, 20 , 20);
        }
        graphics.presentScene();  
    }

    cleanUp(dunn, ronaldo, ball, bat);

    graphics.quit();
    return 0;
}

