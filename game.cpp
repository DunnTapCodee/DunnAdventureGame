#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameObject.h"
#include "MainCharacter.h"
#include "graphics.h"
#include "Monster.h"
#include "MonsBall.h"
#include "menu.h"

#include <vector>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

Graphics graphics;
SDL_Event event;
SDL_Texture* background;
SDL_Texture* current_score = nullptr;
SDL_Texture* high_score = nullptr;

    Mix_Music *intro;
    Mix_Music *ingame;
    Mix_Chunk* moving;
    Mix_Chunk* hit;
    Mix_Chunk* shootingball;
    Mix_Chunk* jump;
    Mix_Chunk* gameover;
    Mix_Chunk* touch;
    Mix_Chunk* click;

    Menu* menu = new Menu("media/menu.png", graphics);
bool running = true;
bool moving_left = false, moving_right = false, jumping = false,
     lastMovingLeft = false, lastMovingRight = false;
bool GameOver = false;
Uint32 lastMoveTime = SDL_GetTicks();

    MainCharacter* dunn;
    MainCharacter* ronaldo;
    GameObject* ball;
    Monster* bat;

    vector<GameObject*> obj;
    vector<Monster* > mons;
    vector<MonsBall* > monsball;

    template <typename T>

void freeVector(vector<T*>& vec) {
    for (auto& ptr : vec) {
        SDL_DestroyTexture(ptr->texture);
        ptr->texture = nullptr;
        delete ptr;
    }
    vec.clear();
}

void freeAudioResources() {
    if (intro) { Mix_FreeMusic(intro); intro = nullptr; }
    if (ingame) { Mix_FreeMusic(ingame);ingame = nullptr;}
    if (moving) {Mix_FreeChunk(moving);moving = nullptr;}
    if (hit) {Mix_FreeChunk(hit);hit = nullptr; }
    if (shootingball) {Mix_FreeChunk(shootingball); shootingball = nullptr;}
    if (jump) {Mix_FreeChunk(jump);jump = nullptr;}
    if (touch) { Mix_FreeChunk(touch); touch = nullptr;}
    if (click) { Mix_FreeChunk(click); click = nullptr;}
    if (gameover) { Mix_FreeChunk(gameover); gameover = nullptr;}
}

void cleanUp(MainCharacter* dunn, MainCharacter* ronaldo, GameObject* ball, Monster* bat ) {
    freeVector(obj);
    freeVector(mons);
    freeVector(monsball);

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
            graphics.play(shootingball);
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
                    case SDLK_SPACE: jumping = true; graphics.play(jump); break;
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
    if (moving_right) { dunn->move_right(); ronaldo->move_right();}
    if (jumping && dunn->is_touching_ground()) {
        dunn->jump(JUMP_SPEED);
    }
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
            if ( dunn-> is_touching_ground()) graphics.play(moving);
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
                graphics.play(touch);
                score += 5;
                delete *it;
                it = mons.erase(it);
                erased = true;
                continue;   
           }
        for (auto itr = obj.begin(); itr != obj.end();)
        {
            if ( abs((*itr)->x - (*it)->x ) <= 20  && abs((*itr)->y - (*it)->y ) <= 20 )
            {
                graphics.play(hit);
                score += 2;
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
                change = true; score -= 8;

           }
        else if ( dunn->overMapRight ) 
            {
                dunn -> x = 0;
                ronaldo -> x = 100;
                dunn -> overMapRight = false;
                change = true; score -=8;
            }
   }

void CreateMonsBall( vector <Monster* > mons, int &current_distance)
  {
        current_distance += 5; 
        for (auto &it : mons)
          {
              if (current_distance >= it->distance_change)
                  {
                          current_distance = 0;
                          it->distance_change = ( rand() % 50 ) + 100;
                          MonsBall* bon = new MonsBall ("media/monsball.png", graphics, it->x , it->y);
                          monsball.push_back(bon);
                  }
          }
    }

void UpdateMonsBall(MainCharacter* dunn, vector<MonsBall*>& monsball) {
        for (auto it = monsball.begin(); it != monsball.end();) {
            (*it)->update();
            bool eraseball = false;
    
            if ((*it)->x - dunn->x >= 0 && (*it)->x - dunn->x <= 50 && (*it)->y - dunn->y >= 0 && (*it)->y - dunn->y <= 80)
              {
                GameOver = true;
                delete *it;
                it = monsball.erase(it);
                eraseball = true;
                break;
            }
    
            if ((*it)->reachedTarget) {
                delete *it;
                it = monsball.erase(it);
                eraseball = true;
            } else {
                ++it;
            }
        }
    }
    
void StartGame(Menu* menu, Mix_Music* intro)
  {
    while (running && menu->inMenu)
    {
        // running intro music
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) graphics.play(intro);

        graphics.renderTexture(menu->texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        graphics.presentScene();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { running = false; break; }
            else if (event.type == SDL_MOUSEBUTTONDOWN && !menu->selectingPlayer)
             {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                menu->handleMenuClick(mouseX, mouseY);
                graphics.play(click);
                // if (menu->selectingPlayer) break;
             }
            if (menu->selectingPlayer) 
             {
                menu->texture = graphics.loadTexture("media/player.jpg", graphics.renderer);
                graphics.renderTexture(menu->texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                graphics.presentScene();
                
                // Cập nhật lại tọa độ chuột
                bool waitingForPlayerSelect = true;
                while (waitingForPlayerSelect)
                 {
                    while (SDL_PollEvent(&event))
                     {
                        if (event.type == SDL_MOUSEBUTTONDOWN)
                         {
                            graphics.play(click);
                            SDL_GetMouseState(&mouseX, &mouseY);
                            menu->handlePlayerSelectClick(mouseX, mouseY);
                            waitingForPlayerSelect = false;
                        }
                    }
                }
             }
            
        }
    }
    Mix_FreeMusic(intro);
    
  }

void GameOverr(Menu* menu, MainCharacter* dunn, MainCharacter* ronaldo, bool &GameOver)
  {
    score = 0;
    // vẽ gameover
    menu->texture = graphics.loadTexture("media/gameover.png", graphics.renderer);
    graphics.renderTexture(menu->texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    graphics.presentScene();
    
    // chờ cho chọn nhân vật hoặc chơi game mới
    bool waitingForInput = true;
    while (waitingForInput)
    {
        // cho nhân vật dừng
        moving_left = moving_right = jumping = false;
        lastMovingLeft = lastMovingRight = false;


        while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT) { running = false; waitingForInput = false; break; }
                else if (event.type == SDL_MOUSEBUTTONDOWN && !menu->selectingPlayer)
                        {
                                graphics.play(click);
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                menu->handleContinuePlay(mouseX, mouseY);
                                // if (menu->selectingPlayer) break;
                         }
            }
        if (menu->selectingPlayer) 
                {
                    menu->texture = graphics.loadTexture("media/player.jpg", graphics.renderer);
                    graphics.renderTexture(menu->texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    graphics.presentScene();

                        // Cập nhật lại tọa độ chuột
                    bool waitingForPlayerSelect = true;
                    while (waitingForPlayerSelect)
                        {
                            while (SDL_PollEvent(&event))
                                {
                                    if (event.type == SDL_MOUSEBUTTONDOWN)
                                        {
                                            graphics.play(click);
                                            SDL_GetMouseState(&mouseX, &mouseY);
                                            menu->handlePlayerSelectClick(mouseX, mouseY);
                                            waitingForPlayerSelect = false;
                                        }
                                    }
                        }
                    waitingForInput = false;
                    // đổi nhân vật
                    if ( menu->ronaldo == true) ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
                    else ronaldo = new MainCharacter("media/messi.png", graphics, 200, GROUND_LEVEL); 
                }
        else if (menu->inMenu) { waitingForInput = false; menu->inMenu = false;}
    }
   GameOver = false;
   freeVector(obj); freeVector(monsball); freeVector(mons);
   dunn->x = 100; dunn->y = GROUND_LEVEL; dunn->texture = dunn->framesRight[0];
   ronaldo->x = 200;
  }

void RenderObject(Monster* bat, vector<Monster* > mons, vector <GameObject* > obj, vector < MonsBall* > monsball , int& distance, int &FRAME_CHANGE_DISTANCE_MONS, int &framesMons)
 {
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

    for (auto& bon : monsball) {
            graphics.renderTexture(bon->texture, bon->x, bon->y, 20 , 20);
        }
 }

int loadHighScore(const string&filename)
   {
        ifstream file(filename);
        int highscore = 0;
        if (file.is_open())
          {
                file >> highscore;
                file.close();
          }
        return highscore;
   }

void saveHighScore(const string& filenmae, int &score)
   {
        ofstream file ("cpp/highscore.txt");
        if (file.is_open())
            {
                file << score;
                file.close();
            }
   }

void LoadScoreTextures()
 {
     current_score = graphics.loadTexture("media/score.png", graphics.renderer);
     high_score = graphics.loadTexture("media/highscore.png", graphics.renderer);
 }
 
void RenderScore()
 {
     int highscore = loadHighScore("cpp/highscore.txt");
     if (score > highscore) { saveHighScore( "cpp/highscore.txt", score); highscore = score;}
     graphics.renderTexture(current_score, 30, 20, 320, 27);
     graphics.renderTexture(high_score, 450, 20, 320, 27);
     string readScores = to_string(score);int space = 0;
     while (!readScores.empty() )
       {
            int number = readScores[0] - '0';
            graphics.renderTexture(dunn->Scores[number], 220 + space, 23, 22, 22);
            space += 22;
            readScores.erase(0, 1); 
       }
    string readsHighScores = to_string(highscore);space = 0;
    while (!readsHighScores.empty() )
        {
            int number = readsHighScores[0] - '0';
            graphics.renderTexture(dunn->Scores[number], 640 + space, 23, 22, 22);
            space += 22;
            readsHighScores.erase(0, 1); 
        }
    
 }

void CleanupScoreTextures()
 {
     SDL_DestroyTexture(current_score); current_score = nullptr;
     SDL_DestroyTexture(high_score); high_score = nullptr;
     for (auto& texture : dunn->Scores) SDL_DestroyTexture(texture);
     dunn->Scores.clear();
 }
 
  // Main function
int main(int argc, char* argv[]) {
    graphics.init();

    intro = graphics.loadMusic("media/intro.mp3");
    ingame = graphics.loadMusic("media/ingame.mp3");
    moving = graphics.loadSound("media/moving.wav");
    jump = graphics.loadSound("media/jump.wav");
    shootingball = graphics.loadSound("media/shootingball.wav");
    hit = graphics.loadSound("media/hit.wav");
    touch = graphics.loadSound("media/touch.wav");
    click = graphics.loadSound("media/click.wav");
    gameover = graphics.loadSound("media/gameover.wav");
    Menu* menu = new Menu("media/menu.png", graphics);
    
    StartGame(menu, intro); // Menu Game
    
    dunn = new MainCharacter("media/dunn.png", graphics, 100, GROUND_LEVEL);
    dunn-> loadFrameLeft( { "media/dunnleft.png", "media/dunn2left.png", "media/dunn3left.png", "media/dunn2left.png", "media/dunn2jump.png"} );
    dunn-> loadFrameRight( {"media/dunn.png", "media/dunn2.png", "media/dunn3.png", "media/dunn2.png", "media/dunnjump.png"} );
    dunn ->loadMaps( {"media/background.jpg","media/background2.jpg", "media/background3.jpg", "media/background4.jpg", "media/background5.jpg", "media/background6.jpg", "media/background7.jpg"});
    dunn ->loadScores({"media/0.png", "media/1.png", "media/2.png", "media/3.png", "media/4.png", "media/5.png","media/6.png", "media/7.png", "media/8.png", "media/9.png"});
    LoadScoreTextures();

    

    Monster* bat = new Monster("media/obj1.png", graphics, 100, 100);
     bat ->loadFrame( { "media/obj1.png", "media/obj2.png", "media/obj3.png" } );
    printf("Monster frames loaded: %lu\n", bat->frames.size());

    if ( menu->ronaldo == true) ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
    else ronaldo = new MainCharacter("media/messi.png", graphics, 200, GROUND_LEVEL); 
    ball = new GameObject("media/ball.png", graphics, 150, GROUND_LEVEL);


    if ( !menu -> inMenu) 
    {
        SDL_Texture* background = graphics.loadTexture("media/background.jpg", graphics.renderer);
        graphics.background = background; graphics.play(ingame);

        // Các biến để thay đổi frames của Monster
        int framesMons = 0, distance = 0, FRAME_CHANGE_DISTANCE_MONS = 40;// Biến để Monster thả ball
        int current_distance = 0;/*Biến để Monster thả ball*/ int width = 50;

        while (running)
            {
                graphics.presentScene();
                clock_t start = clock();
                Moving(); bool change = false;
                changeMap( dunn, ronaldo, change);
                if (change == true)
                    {
                        int map = ( rand() % 7 );
                        graphics.background = dunn ->Maps[map];
                        freeVector(obj); freeVector(mons); freeVector(monsball);
                        graphics.prepareScene(graphics.background);
                    }
        
                UpdateBalls();
                while ( mons.size() <= 3) { FirstMonster(); }
        
                UpdateMonster();

                CreateMonsBall(mons, current_distance);

                UpdateMonsBall(dunn, monsball);

                if (score < 0) GameOver = true;
                if (GameOver)
                {
                        graphics.play(gameover);
                        GameOverr(menu, dunn, ronaldo, GameOver);
                        if ( menu->ronaldo == true) ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
                            else ronaldo = new MainCharacter("media/messi.png", graphics, 200, GROUND_LEVEL); 
                }

                HandleCharacterMovement(dunn, moving_left, moving_right, lastMovingLeft, lastMovingRight, width);
        
                RenderObject(bat, mons, obj, monsball, distance, FRAME_CHANGE_DISTANCE_MONS, framesMons);

                RenderScore(); 
                int used_time = clock() - start; SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
            }
    }
    cleanUp(dunn, ronaldo, ball, bat);CleanupScoreTextures();
    
    graphics.quit();
    return 0;
}