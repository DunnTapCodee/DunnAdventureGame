#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"


struct Graphics
{
    SDL_Renderer* renderer;
    SDL_Window* window;

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

    void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
      SDL_SetRenderDrawColor(renderer, 150, 100, 50, 130);
      for (int w = 0; w < radius * 2; w++) {
          for (int h = 0; h < radius * 2; h++) {
              int dx = radius - w; // Khoảng cách x từ tâm
              int dy = radius - h; // Khoảng cách y từ tâm
              if ((dx * dx + dy * dy) <= (radius * radius)) {
                  SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
              }
          }
      }
  }

    void rectangle (SDL_Renderer* renderer, int x, int y, int w, int h)
    {
       SDL_Rect rect;
       rect.x = x;
       rect.y = y;
       rect.w = w;
       rect.h = h;
       SDL_SetRenderDrawColor(renderer, 150, 100, 50, 130);
       SDL_RenderFillRect(renderer, &rect);
    }
  
    void circle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = 0, y = radius;
    int d = 1 - radius; // Giá trị quyết định 
    SDL_SetRenderDrawColor(renderer, 60, 70, 30, 40);
    while (x <= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

    void clearScr(SDL_Renderer* renderer)
     {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0 , 255);
        SDL_RenderClear(renderer);
     }

    void renderTexture (SDL_Texture* texture, int x, int y, SDL_Renderer* renderer,  float scale = 0.01f)
    {
    
      SDL_Rect dest;

      dest.x = x;
      dest.y = y;
      SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
      dest.w = static_cast<int>(dest.w * scale);
      dest.h = static_cast<int>(dest.h * scale);
      

      SDL_RenderCopy(renderer, texture, NULL, &dest);

      // SDL_Log("Texture size - Width: %d, Height: %d", dest.w, dest.h);

    }

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
