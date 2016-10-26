//
// floating-menu.cpp
//

#include <iostream>
#include <SDL2/SDL.h>

#include <fcntl.h>
#include <linux/input.h>

using namespace std;

#define IMAGE_FILENAME "images/home.bmp"

int main(int argc, char *argv[])
{

    bool grab=false;

    if ((argc > 1) && !(strcmp(argv[1], "--grab"))) {
        printf ("enabling grab\n");
        grab=true;
    }


    if (grab) {
        // grab keyboard & mouse
        int fevdev0, result;
        fevdev0=open("/dev/input/event1", O_RDONLY);
        if (fevdev0) {
            result=ioctl(fevdev0, EVIOCGRAB, 1);
            printf ("grab dev0 rc=%d\n", result);
        }
    }


    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // top left window
    SDL_Window *win = SDL_CreateWindow("FloatingMenu", 0, 0, 128, 128, /* SDL_WINDOW_HIDDEN */ SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    else {
        SDL_SetWindowGrab(win, SDL_TRUE);
        //SDL_WarpMouse(0, 0);
        SDL_PumpEvents();
        //SDL_WM_GrabInput(SDL_GRAB_ON);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::string imagePath = IMAGE_FILENAME;
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);

    SDL_Event event;
    //float frotate=0.0f;
    int delay=10; // milliseconds
    bool runrun=true;


    while (runrun) {

      SDL_RenderCopyEx(ren, tex, NULL, NULL, 0L, NULL, SDL_FLIP_HORIZONTAL);
      SDL_RenderPresent(ren);
      SDL_Delay(delay);

      while (SDL_PollEvent (&event)) {
          switch (event.type)
              {
              case SDL_KEYDOWN:
                  if (event.key.keysym.sym == SDLK_ESCAPE) {
                      printf("byebye\n");
                      SDL_Quit();
                      runrun=false;
                  }
                  
              case SDL_MOUSEMOTION: //SDL_MouseMotionEvent:
                  printf ("mouse moving\n");
                  break;
              }
      }
    }

    SDL_Quit();
    return 0;
}
