#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Dunn Tap Code";
const int SPEED = 100;

#endif

#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"

struct Graphics
{
    SLD_Renderer* renderer;
    SDL_Window* window;

    void LogErrorAndExit(char* msg, char* error)
      {
              SDL_LogMessage (SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s : %s", msg, error);
              SDL_Quit();
      }

    void init()
      {
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
                {
                    LogErrorAndExit( "SDL_Init" , SDL_GetError());

                }
            window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

            if (window == nullptr)
               {
                 LogErrorAndExit("SDL_CreateWindow", SDL_GetError());

               }
            if ( IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG))
               LogErrorAndExit("SDL_image error", IMG_GetError());
            
            renderer = SDL_CreateRenderer(window, -1 ,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (renderer == nullptr) 
               LogErrorAndExit("SDL_CreateRernder", SDL_GetError());
            
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
      }

    void prepareScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture* loadTexture (const char* filename, SDL_Renderer* renderer)
      {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename );

            SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
            if (texture == nullptr)
              SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_CATEGORY_ERROR, "Load texture %s", IMG_GetError());
            return texture;
      }

    void clear()
       {
         SDL_SetRenderDrawColor(renderer, 255, 0 , 0 , 255);
         SDL_RenderClear(renderer);
       }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

 };

 #endif

 #ifndef GAMEOBJECT_H
 #define GAMEOBJECT_H

 #include <SDL2/SDL.h>
 #include "graphics.h"

 class GameObject {
    public:
        int x, y;
        int velocity;
        SDL_Texture* texture;

        GameObject (const char*filePath, Graphics& graphics, int startX, int startY, int vel)
        : x (startX) , y (startY), velocity (vel)
            {
                texture = graphics.loadTexture(filePath, graphics.renderer);
            }
        ~GameObject()
            {
                SDL_DestroyTexture(texture);
            }

        void move(int dx, int dy)
            {
                x = x + dx * velocity;
                y = y + dy * velocity;
            }

        void render( Graphics& graphics)
            {
                graphics.renderTexture( texture, x, y, graphics.renderer, 0.1f);
            }

        void renderTerrain (Graphics& graphics)
            {
                graphics.renderTexture( texture, x, y, graphics.renderer);
            }

    }

#endif

#ifndef GAME_H
#define GAME_h

#include <SDL2/SDL.h>
#include "graphics.h"
#include "GameObject.h"
#include <vector>

class Game
{
    public:
        Graphics graphics;
        SDL_Texture* background;
        std::vector<GameObject*> objects;

        Game()
        {
            graphics.init();
            background = graphics.loadTexture("media/bikiniBottom.jpg");
            
        }
}