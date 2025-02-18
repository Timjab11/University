#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dynamic_array.h"
#include "sdl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    SDL_Texture* texture;
    SDL_Rect dstRect;
    double angle;
    SDL_Point center;
} Snowflake;

int main() {
    SDL_Context ctx = sdl_context_init("SDL experiments", WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Window* win = ctx.window;
    SDL_Renderer* ren = ctx.renderer;
    SDL_Texture* snowflake_texture = IMG_LoadTexture(ren, "snowflake.svg");

    dynarray rect_arr;
    dynarray_init(&rect_arr, 64);

    SDL_Event e;
    bool quit = false;
    bool byclick = true;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                if (byclick) {
                    Snowflake* snowflake = (Snowflake*)malloc(sizeof(Snowflake));
                    snowflake->texture = snowflake_texture;
                    snowflake->angle = 0;
                    int randomSize = rand() % 31 + 20;
                    snowflake->dstRect.w = randomSize;
                    snowflake->dstRect.h = randomSize;
                    snowflake->dstRect.x = e.button.x - snowflake->dstRect.w / 2;
                    snowflake->dstRect.y = e.button.y - snowflake->dstRect.h / 2;
                    snowflake->center.x = snowflake->dstRect.w / 2;
                    snowflake->center.y = snowflake->dstRect.h / 2;
                    dynarray_push(&rect_arr, snowflake);
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (!byclick) {
                    Snowflake* snowflake = (Snowflake*)malloc(sizeof(Snowflake));
                    snowflake->texture = snowflake_texture;
                    snowflake->angle = 0;
                    int randomSize = rand() % 31 + 20;
                    snowflake->dstRect.w = randomSize;
                    snowflake->dstRect.h = randomSize;
                    snowflake->dstRect.x = e.button.x - snowflake->dstRect.w / 2;
                    snowflake->dstRect.y = e.button.y - snowflake->dstRect.h / 2;
                    snowflake->center.x = snowflake->dstRect.w / 2;
                    snowflake->center.y = snowflake->dstRect.h / 2;
                    dynarray_push(&rect_arr, snowflake);
                }
            }
        }

        SDL_RenderClear(ren);

        for (size_t i = 0; i < rect_arr.size; i++) {
            Snowflake* snowflake = (Snowflake*)rect_arr.items[i];
            snowflake->angle++;
            snowflake->dstRect.y++;
            if (snowflake->dstRect.y > WINDOW_HEIGHT) {
                dynarray_remove(&rect_arr, snowflake);
                continue;
            }
            SDL_RenderCopyEx(ren, snowflake->texture, NULL, &snowflake->dstRect, snowflake->angle, &snowflake->center, SDL_FLIP_NONE);
        }

        SDL_RenderPresent(ren);
    }

    dynarray_free(&rect_arr);
    SDL_DestroyTexture(snowflake_texture);
    sdl_context_free(&ctx);

    return 0;
}
