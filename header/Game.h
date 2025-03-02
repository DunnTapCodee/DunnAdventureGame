#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "graphics.h"
#include "GameObject.h"

class Game {
public:
    Graphics graphics;
    SDL_Texture* background;
    std::vector<GameObject*> objects; // Danh sách vật thể

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

    void update(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            for (auto obj : objects) { // Lặp qua tất cả vật thể
                switch (event.key.keysym.sym) {
                    case SDLK_a: obj->move(-1, 0); break;
                    case SDLK_d: obj->move(1, 0); break;
                    case SDLK_w: obj->move(0, -1); break;
                    case SDLK_s: obj->move(0, 1); break;
                }
            }
        }
    }

    void render() {
        SDL_RenderClear(graphics.renderer);
        SDL_RenderCopy(graphics.renderer, background, NULL, NULL); // Vẽ background
        for (auto obj : objects) {
            obj->render(graphics); // Vẽ tất cả vật thể
        }
        graphics.presentScene();
    }
};

#endif
