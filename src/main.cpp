#include <SDL2/SDL.h>
#include <iostream>
#include <inttypes.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

#define WIDTH 256
#define HEIGHT 240

void gui_init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

    window = SDL_CreateWindow("NESsxt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
}
void gui_render()
{
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
void gui_run()
{
    SDL_Event e;
    uint32_t frameStart, frameTime;
    const int FPS   = 60;
    const int DELAY = 1000.0f / FPS;

    while (true) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e))
            switch (e.type)
            {
            case SDL_QUIT: return;
            
            default:
                break;
            }
        gui_render();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY)
            SDL_Delay((int)(DELAY - frameTime));
    }

}

int main(int _argc, char** _argv)
{
    gui_init();
    gui_run();
}