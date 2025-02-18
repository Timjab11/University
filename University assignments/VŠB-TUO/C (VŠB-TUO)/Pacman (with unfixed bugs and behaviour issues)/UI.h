#pragma once
#include "localizer.h"
#include "sdl.h"
typedef struct{
Position position;
SDL_Surface* text;
SDL_Texture* texture;
} Button;