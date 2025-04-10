#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameObject.h"
#include "MainCharacter.h"
#include "graphics.h"
#include "Monster.h"
#include "MonsBall.h"
#include "MonsterBlue.h"
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
SDL_Texture* upgrade = nullptr;
SDL_Texture* upspeed = nullptr;
SDL_Texture* upball = nullptr;
SDL_Texture* upjump = nullptr;
SDL_Texture* upthrough = nullptr;
SDL_Texture* upheart = nullptr;
SDL_Texture* stop = nullptr;


    Mix_Music* intro;
    Mix_Music* ingame;
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
bool Fupgrade = false;
bool Fupspeed  = false;
bool Fupjump  = false;
bool Fupthrough  = false;
bool Fupball  = false;
bool Fupheart = false;
bool pause = false;
Uint32 lastMoveTime = SDL_GetTicks();

    MainCharacter* dunn;
    MainCharacter* ronaldo;
    GameObject* ball;
    Monster* bat;
    MonsterBlue* blue;


    vector<GameObject*> obj;
    vector<Monster* > mons;
    vector<MonsBall* > monsball;
    vector<MonsterBlue* > monsblue;

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
    freeVector(monsblue);

    for (auto& texture : dunn->framesLeft) SDL_DestroyTexture(texture);
    dunn->framesLeft.clear();

    for (auto& texture : dunn->framesRight) SDL_DestroyTexture(texture);
    dunn->framesRight.clear();

    for (auto& texture : bat->frames) SDL_DestroyTexture(texture);
    bat->frames.clear();

    SDL_DestroyTexture(current_score); SDL_DestroyTexture(high_score);SDL_DestroyTexture(upgrade);SDL_DestroyTexture(upspeed);
    SDL_DestroyTexture(upball);SDL_DestroyTexture(upthrough);SDL_DestroyTexture(upjump);SDL_DestroyTexture(upheart);SDL_DestroyTexture(stop);
    delete dunn; dunn = nullptr;
    delete ronaldo; ronaldo = nullptr;
    delete ball; ball = nullptr;
}


bool ball_shot = false; int mouseX, mouseY;

void ClickToAddFeature(SDL_Event& event, int& score)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= 373 && mouseX <= 433 && mouseY >= 10 && mouseY <= 70) {
            pause = true; graphics.play(click);
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 67 && mouseY <= 127) {
            if (score >= 100) {
                Fupgrade = true; graphics.play(click);
                score -= 100;
            }
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 127 && mouseY <= 187) {
            if (score >= 30) {
                Fupspeed = true; graphics.play(click);
                score -= 30;
            }
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 187 && mouseY <= 247) {
            if (score >= 30) {
                Fupball = true; graphics.play(click);
                score -= 30;
            }
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 247 && mouseY <= 307) {
            if (score >= 30) {
                Fupthrough = true; graphics.play(click);
                score -= 60;
            }
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 307 && mouseY <= 367) {
            if (score >= 30) {
                Fupjump = true; graphics.play(click);
                score -= 30;
            }
        }
        else if (mouseX >= 720 && mouseX <= 780 && mouseY >= 367 && mouseY <= 427) {
            if (score >= 30) {
                HEART++; graphics.play(click);
                score -= 30;
            }
        }
    }
}

void ResetFeature()
  {
    MaxBall = 3;
    MOVE_SPEED =  5;
    JUMP_SPEED = 35;
    HEART = 3;
    THROUGH = 0;
  }

void UpdateFeature()
{
    if (Fupgrade)
    {
            Fupball = true;
            Fupthrough = true;
            Fupjump = true;
            Fupspeed  =true;
            Fupgrade = false;
    }

    if (Fupspeed) {MOVE_SPEED+= 2; Fupspeed = false;}
    if (Fupjump) { JUMP_SPEED += 5; Fupjump = false;}
    if (Fupball) { MaxBall++; Fupball = false;}
    if (Fupthrough) { THROUGH++; Fupthrough = false;}
}

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

            // through của ball
            if (obj.size() < MaxBall) {  obj.push_back(newBall); graphics.play(shootingball);}

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
            ShootingBall(event); // Xử lý bắn bóng khi chuột được nhấn
            ClickToAddFeature(event, score);  
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

void FirstMonsterBlue()
  {
        int randomX = rand() % 200 + (dunn->x + 400 ) % SCREEN_WIDTH;
        int randomY = 450;
        int TargetX =  dunn->x; 
        MonsterBlue* blue = new MonsterBlue("media/blue.png", graphics, randomX , randomY);
        monsblue.push_back(blue);
        blue -> setTarget(TargetX);
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
                if ( (*itr)->through-- ) 
                {
                    delete *itr;
                    itr = obj.erase(itr);
                }
    
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

void UpdateMonsterBlue()
  {
    for (auto it = monsblue.begin(); it != monsblue.end();)
    {
        (*it)->update();
        if ((*it)->reachedTarget == true)
        {
            int TargetX = rand() % SCREEN_WIDTH;
            // printf("Monster reached target! New target: %d\n", TargetX);
            (*it)->setTarget(TargetX);
        }
        bool erased = false;
        if ( ( ( dunn->x - (*it)->x ) <= 0 && ( (*it)->x - dunn->x <= 40 ) ) && ( dunn -> y - 100 <= (*it)->y ) )
            {
                if (HEART){
                graphics.play(touch);delete *it;
                it = monsblue.erase(it);erased = true;
                HEART--;continue;   
                }
                else GameOver = true;
            }
        for (auto itr = obj.begin(); itr != obj.end();)
        {
           if ( (( (*itr)->x - (*it)->x ) <= 60  &&  ( (*it)->x - (*itr)->x ) <= 0 ) && ( (*it)->y  <= (*itr)->y  ) && ( (*itr)->y <= (*itr)->y + 100 ) )
            {
                graphics.play(hit);
                score += 2;
                if ( (*itr)->through <=0) 
                {
                    delete *itr;
                    itr = obj.erase(itr);
                }
                else (*itr)->through--;
    
                delete *it;
                it = monsblue.erase(it);
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
                if (HEART) { HEART--;}
                else GameOver = true;
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
    ResetFeature();
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
freeVector(obj); freeVector(monsball); freeVector(mons);freeVector(monsblue);
dunn->x = 100; dunn->y = GROUND_LEVEL; dunn->texture = dunn->framesRight[0];
ronaldo->x = 200;
}

void PauseGame(Menu* menu)
{
    if (stop) 
      {
        menu->texture = graphics.loadTexture("media/pause.png", graphics.renderer);
        graphics.renderTexture(menu->texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        graphics.presentScene();
        bool waitingForInput = true;
        while (waitingForInput)
        {
            while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT) { running = false; waitingForInput = false; break; }
                    else if (event.type == SDL_MOUSEBUTTONDOWN && !menu->selectingPlayer)
                            {
                                    graphics.play(click);
                                    int mouseX, mouseY;
                                    SDL_GetMouseState(&mouseX, &mouseY);
                                    menu->Continue(mouseX, mouseY);
                                    if (menu->continueGame) { waitingForInput = false; graphics.play(click); pause = false;menu->continueGame = false; break;}
                                    else if (menu->exitGame) {running = false; waitingForInput = false;graphics.play(click); pause = false; break; }
                                    // if (menu->selectingPlayer) break;
                            }
                }
        }
    }
}

void RenderObject(Monster* bat, vector<Monster* > mons, vector <GameObject* > obj, vector <MonsterBlue* > monsblue, vector < MonsBall* > monsball , int& distance, int &FRAME_CHANGE_DISTANCE_MONS, int &framesMons)
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
    for (auto& blue : monsblue) {
            graphics.renderTexture(blue->texture, blue->x, blue->y, 60 , 60);
        }
    
}

int  loadHighScore(const string&filename)
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

void LevelFeature(int level, int x, int y)
  {
    string readScores = to_string(level);int space = 0;
    while (!readScores.empty() )
    {
            int number = readScores[0] - '0';
            graphics.renderTexture(dunn->Scores[number], x + space , y , 10, 10);
            space += 8;
            readScores.erase(0, 1); 
    }
  }

void RenderScoreAndFeature()
{
    graphics.renderTexture(upgrade, SCREEN_WIDTH - 80, 67, 60, 60);
    graphics.renderTexture(upspeed, SCREEN_WIDTH - 80, 127, 60, 60);LevelFeature(MOVE_SPEED, SCREEN_WIDTH - 75, 135);
    graphics.renderTexture(upball, SCREEN_WIDTH - 80, 187, 60, 60);LevelFeature(MaxBall, SCREEN_WIDTH - 75, 195);
    graphics.renderTexture(upthrough, SCREEN_WIDTH - 80, 247, 60, 60);LevelFeature(THROUGH, SCREEN_WIDTH - 75, 255);
    graphics.renderTexture(upjump, SCREEN_WIDTH - 80, 307, 60, 60);LevelFeature(JUMP_SPEED, SCREEN_WIDTH - 75, 315);
    graphics.renderTexture(upheart, SCREEN_WIDTH - 80, 367, 60, 60);LevelFeature(HEART, SCREEN_WIDTH - 75, 375);
    graphics.renderTexture(stop, 373, 10, 60, 60);

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

void loadMusicsAndFeature()

{
    intro = graphics.loadMusic("media/intro.mp3");
    ingame = graphics.loadMusic("media/ingame.mp3");
    moving = graphics.loadSound("media/moving.wav");
    jump = graphics.loadSound("media/jump.wav");
    shootingball = graphics.loadSound("media/shootingball.wav");
    hit = graphics.loadSound("media/hit.wav");
    touch = graphics.loadSound("media/touch.wav");
    click = graphics.loadSound("media/click.wav");
    gameover = graphics.loadSound("media/gameover.wav");
    upgrade = graphics.loadTexture("media/upgrade.png", graphics.renderer);
    upspeed = graphics.loadTexture("media/upspeed.png", graphics.renderer);
    upball = graphics.loadTexture("media/upball.png", graphics.renderer);
    upthrough = graphics.loadTexture("media/upthrough.png", graphics.renderer);
    upjump = graphics.loadTexture("media/upjump.png", graphics.renderer);
    stop = graphics.loadTexture("media/stop.png", graphics.renderer);
    upheart = graphics.loadTexture("media/upheart.png", graphics.renderer);
}

void InitGame(Menu*& menu) {
    graphics.init();
    loadMusicsAndFeature();
    menu = new Menu("media/menu.png", graphics);
    StartGame(menu, intro);
}

void LoadMainCharacters(Menu* menu) {
    dunn = new MainCharacter("media/dunn.png", graphics, 100, GROUND_LEVEL);
    dunn->loadFrameLeft({ "media/dunnleft.png", "media/dunn2left.png", "media/dunn3left.png", "media/dunn2left.png", "media/dunn2jump.png" });
    dunn->loadFrameRight({ "media/dunn.png", "media/dunn2.png", "media/dunn3.png", "media/dunn2.png", "media/dunnjump.png" });
    dunn->loadMaps({ "media/background.jpg", "media/background2.jpg", "media/background3.jpg", "media/background4.jpg", "media/background5.jpg", "media/background6.jpg", "media/background7.jpg" });
    dunn->loadScores({ "media/0.png", "media/1.png", "media/2.png", "media/3.png", "media/4.png", "media/5.png", "media/6.png", "media/7.png", "media/8.png", "media/9.png" });
    LoadScoreTextures();

    if ( menu->ronaldo == true) ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
    else ronaldo = new MainCharacter("media/messi.png", graphics, 200, GROUND_LEVEL); 
    ball = new GameObject("media/ball.png", graphics, 150, GROUND_LEVEL);

}

void SetupMonster(Monster*& bat) {
    bat = new Monster("media/obj1.png", graphics, 100, 100);
    bat->loadFrame({ "media/obj1.png", "media/obj2.png", "media/obj3.png" });
    printf("Monster frames loaded: %lu\n", bat->frames.size());
}

void RunGameLoop(Menu* menu, Monster* bat) {
    graphics.background = graphics.loadTexture("media/background.jpg", graphics.renderer);
    graphics.play(ingame);

    int framesMons = 0, distance = 0;
    int FRAME_CHANGE_DISTANCE_MONS = 40;
    int current_distance = 0;
    int width = 50; int spawn_time = 100;

    while (running) {
        graphics.presentScene();
        clock_t start = clock();

        UpdateFeature();
        Moving();

        bool change = false;
        changeMap(dunn, ronaldo, change);
        if (change) {
            int map = rand() % 7;
            graphics.background = dunn->Maps[map];
            freeVector(obj); freeVector(mons); freeVector(monsball);freeVector(monsblue);
            graphics.prepareScene(graphics.background);
        }

        UpdateBalls();
        while (mons.size() <= 3) FirstMonster();
        
        // Nếu monsblue.size() < giới hạn và đủ 3 giây thì spawn thêm
        static clock_t lastBlueSpawn = clock();
        clock_t now = clock();
        if (monsblue.size() < 3 && (now - lastBlueSpawn) > 3000) {
            FirstMonsterBlue();
            lastBlueSpawn = now;
        }

        UpdateMonster();
        UpdateMonsterBlue();
        CreateMonsBall(mons, current_distance);
        UpdateMonsBall(dunn, monsball);

        if (score < 0) GameOver = true;
        if (GameOver) {
            graphics.play(gameover);
            GameOverr(menu, dunn, ronaldo, GameOver);
            if ( menu->ronaldo == true) ronaldo = new MainCharacter("media/ronaldo.png", graphics, 200, GROUND_LEVEL); 
                else ronaldo = new MainCharacter("media/messi.png", graphics, 200, GROUND_LEVEL); 
        }

        if (pause) PauseGame(menu);

        HandleCharacterMovement(dunn, moving_left, moving_right, lastMovingLeft, lastMovingRight, width);
        RenderObject(bat, mons, obj, monsblue, monsball, distance, FRAME_CHANGE_DISTANCE_MONS, framesMons);
        RenderScoreAndFeature();

        int used_time = clock() - start;
        SDL_Delay(used_time > 16 ? 0 : 16 - used_time);
    }
}

#endif