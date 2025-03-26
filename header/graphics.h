#ifndef _GRAPHICS__H
#define _GRAPHICS__H
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "GameObject.h"


struct Graphics
{
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    SDL_Texture* background = nullptr;


    void LogErrorAndExit(const char* msg, const char* error)
    {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, " %s: %s", msg, error);
            SDL_Quit();
        
    }
   
    void init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
         {
            LogErrorAndExit("SDL_Init", SDL_GetError());

         }
         window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if (window == nullptr) 
          LogErrorAndExit("CreateWindow", SDL_GetError());
        
        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
          LogErrorAndExit ( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer( window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr) LogErrorAndExit("CreateRenderer", SDL_GetError());
        
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void prepareScene(SDL_Texture* background)
    {
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void presentScene()
    {
       SDL_RenderPresent(renderer);
    }

    SDL_Texture* loadTexture (const char* filename, SDL_Renderer* renderer)
      {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);

        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
        if ( texture == NULL)
           SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
      }

    // vẽ vật thể (All Object)
    void renderTexture(SDL_Texture* texture, int x, int y, int width, int height)
       {
            SDL_Rect dest = {x, y, width, height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);
       }

    // vẽ nhận vật chính và nhân vật phụ
    void render(SDL_Texture* characterTexture, int char_x, int char_y, int width, SDL_Texture* characterTexture2, int char_x2, int char_y2)
            {
                  SDL_RenderClear(renderer);
                  SDL_RenderCopy(renderer, background, NULL, NULL); // Vẽ background

                  // Vẽ nhân vật
                  renderTexture(characterTexture, char_x, char_y, width, 120);
                  renderTexture(characterTexture2, char_x2, char_y2, 80, 80);
                  // SDL_RenderPresent(renderer); // Chỉ gọi một lần cuối cùng
            }
        
      

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(background);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
