#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "graphics.h"
#include <iostream>

class Menu {
public:
    bool inMenu = true;
    bool selectingPlayer = false;
    bool ronaldo = true;
    SDL_Texture* texture;

    Menu(const char* filePath, Graphics& graphics) {
        texture = graphics.loadTexture(filePath, graphics.renderer);
    }

    ~Menu() {
        SDL_DestroyTexture(texture);
    }

    void handleMenuClick(int x, int y) {
        if (x >= 380 && x <= 530 && y >= 220 && y <= 320) {
            inMenu = false;
        } else if (x >= 380 && x <= 530 && y >= 350 && y <= 450) {
            selectingPlayer = true;
        }
    }

    void handlePlayerSelectClick(int x, int y) {
        selectingPlayer = false;
        inMenu = false;
        if (x <= SCREEN_WIDTH / 2) ronaldo = false;
        else ronaldo = true;
    }

    void handleContinuePlay(int x, int y) {
        if (x >= 50 && x <= 300 && y >= 450 && y <= 550) {
            inMenu = true;
        } else if (x >= 500 && x <= 750 && y >= 450 && y <= 550) {
            selectingPlayer = true;
        }
    }

};

#endif
