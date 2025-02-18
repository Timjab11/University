#pragma once
#include "localizer.h"
#include "sdl.h"
#include "mapping.h"
typedef enum{
    right,
    left,
    up,
    down,
    none
} Direction;

typedef enum{
    chase,
    scatter,
    frightened,
    eaten
} GhostState;

typedef struct {
    Position originalPosition;
    Position position;
    double movementPercent;
    SDL_Texture* texture;
    Node* closeTo;
    Node* originalNode;
    float speed;
    Direction direction;
    Direction buffer;
    bool chomping;
    int score;
    int lives;
    bool dangerous;
    int dangerousTimer;
    bool dead;
} PacMan;

typedef struct 
{
    SDL_Texture* texture;
    SDL_Texture* frightened;
    Node* originNode;
    Node* closeTo;
    Direction direction;
    GhostState ai_state;
    Position position;
    Position originPosition;
    double movementPercent;
    int forgetTimer;
    float speed;
} Ghost;
