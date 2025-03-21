#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "graphics.h"
#include "GameObject.h"
#include "MainCharacter.h"




class Game {
public:
    Graphics graphics;
    SDL_Texture* background;
    std::vector<GameObject*> objects;
    MainCharacter* dunn = nullptr;

    Game() {
        graphics.init();
        background = graphics.loadTexture("media/bikiniBottom.jpg", graphics.renderer);
    }

    ~Game() {
        SDL_DestroyTexture(background);
        for (auto obj : objects) {
            delete obj;
        }
        graphics.quit();
    }

    void addObject(GameObject* obj) {
        objects.push_back(obj);
    }
    void addMainCharacter(MainCharacter* Player)
      {
          dunn = Player;
      }
    void update(SDL_Event& event) {
        if (!dunn) return;

        if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                       {
                        if ( dunn ->isJumping == false)
                          {
                              dunn ->moveLeft();
                          }
                        break;
                       }
                    case SDLK_d:
                       {
                        if ( dunn ->isJumping == false)
                            {
                                dunn ->moveRight();
                            }
                        break;
                       }
                    case SDLK_SPACE:
                       {
                           const Uint8 *keystate = SDL_GetKeyboardState(NULL);
                           bool moveleft = keystate[SDL_SCANCODE_A];
                           bool moveright = keystate[SDL_SCANCODE_D];
                           dunn ->jump (moveleft, moveright);
                           break;
                       } 
                }
        }
    }

    void render() {
        SDL_RenderClear(graphics.renderer);
        SDL_RenderCopy(graphics.renderer, background, NULL, NULL);
        for (auto obj : objects) {
            obj->render(graphics);
        }

        if (dunn) dunn ->render(graphics);
        graphics.presentScene();
    }
};

#endif
