#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL_ttf.h"
#include "instructions.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

int colorIntensity[] = {0, 85,170,255};

void setPixel(int x, int y) {
    SDL_Rect rect = {x: x<<2, y:y<<2, w:4, h: 4};
    SDL_RenderFillRect(renderer, &rect);
}

void setColor(int color) {
    SDL_SetRenderDrawColor(renderer,
        colorIntensity[(color >> 4) & 3],
        colorIntensity[(color >> 2) & 3],
        colorIntensity[color & 3],
        255
    );
}

void print(char *str, int y) {
    if (NULL == font) {
        return;
    }
    SDL_Color color = {255,255,255};

    SDL_Surface *text = TTF_RenderText_Solid(font, str, color);
    if (NULL != text) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
        int scaleX = 20;
        int scaleY = 40;
        SDL_Rect textSize = {0,y*scaleY, strlen(str)*scaleX,scaleY};
        SDL_RenderCopy(renderer, texture, NULL, &textSize);
        SDL_FreeSurface(text);
        SDL_DestroyTexture(texture);
    } else {
        fprintf(stderr, "Failed to craete text: %s\n", TTF_GetError());
    }
}

int main(int argv, char **argc) {
    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "could not initialize sdl2-ttf: %s\n", TTF_GetError());
    }

    font = TTF_OpenFont("/Library/Fonts/Courier New.ttf", 32);
    if (NULL == font) {
        fprintf(stderr, "error loading font: %s\n", TTF_GetError());
    }

    window = SDL_CreateWindow(
        "hello_sdl2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_INPUT_GRABBED
    );

    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 127,127,127,255);
    for (int y = 0; y < 150; y++) {
        setColor(y % 64);
        for (int x = 0; x < 200; x++) {
            setPixel(x,y);
        }
    }
    print ("Hello world", 0);
    print(getInstruction(0xEA)->mnemonic,1);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                int scancode = event.key.keysym.scancode;
                printf("Scancode %d and name %s\n",
                    scancode,
                     SDL_GetKeyName(event.key.keysym.sym )
                 );
                if (scancode == 41) {
                    running = false;
                }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    printf("Quitting gracefully\n");
    return 0;
}


/*
void drawPixel(int x, int y, Uint32 color) {
    Uint32 *target_pixel = (Uint8 *) screenSurface->pixels + y * screenSurface->pitch +
                                                         x * sizeof *target_pixel;
    *target_pixel = color;
}*/