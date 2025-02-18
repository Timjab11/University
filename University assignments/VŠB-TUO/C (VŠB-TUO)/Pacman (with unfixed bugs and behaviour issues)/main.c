#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "dynamic_array.h"
#include "mapping.h"
#include "entities.h"

typedef enum{
    Main_Menu,
    Options_Menu,
    Game
} GameState;
typedef struct{
    int width;
    int heigth;
} WindowSize;
typedef enum{
    easy,
    normal,
    hard
} difficulty;

bool makeMap(Map* theMap, dynarray* ghosts, PacMan* pacman, int* blockSize, SDL_Renderer* renderer) {
    FILE *file = fopen("map.txt", "r");
    if (!file) {
        printf("Failed to open file\n");
        return false;
    }

    int numLines, lineLength;
    if (fscanf(file, "%d %d\n", &numLines, &lineLength) != 2) {
        fclose(file);
        return false;
    }
    theMap->sizeX = lineLength;
    theMap->sizeY = numLines;
    theMap->graph = (Node*)malloc(sizeof(Node) * numLines * lineLength);
    if (!theMap->graph) {
        printf("Failed to allocate memory for the map\n");
        fclose(file);
        return false;
    }

    bool playerSet = false;

    for (int y = 0; y < numLines; y++) {
        char* theLine = (char*)malloc(sizeof(char) * (lineLength + 2)); // +2 for \n and \0
        if (!fgets(theLine, lineLength + 2, file)) {
            printf("Error reading line from file.\n");
            free(theLine);
            free(theMap->graph);
            fclose(file);
            return false;
        }

        // Remove trailing newline character, if present
        size_t len = strlen(theLine);
        if (len > 0 && theLine[len - 1] == '\n') {
            theLine[len - 1] = '\0';
        }
        for (int x = 0; x < lineLength; x++) {
            Node* node = &theMap->graph[y * lineLength + x];
            node->position.x = x;
            node->position.y = y;
            node->left = NULL;
            node->right = NULL;
            node->up = NULL;
            node->down = NULL;
            node->left = (x > 0) ? &theMap->graph[y * lineLength + (x - 1)] : NULL;
            if (x > 0) {
                theMap->graph[y * lineLength + (x - 1)].right = node; 
            }
            node->up = (y > 0) ? &theMap->graph[(y - 1) * lineLength + x] : NULL;
            node->down = (y < numLines - 1) ? &theMap->graph[(y + 1) * lineLength + x] : NULL;
            switch (theLine[x]) {
                case 'p':
                    if (playerSet) {
                        printf("Error: Multiple players detected.\n");
                        free(theLine);
                        free(theMap->graph);
                        fclose(file);
                        return false;
                    }
                    playerSet = true;
                   pacman->position.x = x * *blockSize;
                   pacman->position.y = y * *blockSize;
                   pacman->direction = none;
                   pacman->speed = 0.25;
                   pacman->buffer = none;
                   pacman->chomping = false;
                   pacman->lives = 3;
                   pacman->dead = false;
                   pacman->originalNode = node;
                   pacman->originalPosition = pacman->position;
                   pacman->dangerous = false;
                    node->state = empty;
                    pacman->closeTo = node;
                    break;
                case 'g':
                    node->state = empty;
                    Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
                    ghost->closeTo = node;
                    ghost->originNode = node;
                    ghost->direction = right;
                    ghost->position.x = x * *blockSize;
                    ghost->position.y = y * *blockSize;
                    ghost->originPosition = ghost->position;
                    ghost->forgetTimer = 5; // customizable per difficulty
                    ghost->speed = 0.10;
                    ghost->texture = IMG_LoadTexture(renderer, "ghost.png");
                    dynarray_push(ghosts,ghost);
                    break;
                case 'x':
                    node->state = wall;
                    break;
                case 'o':
                    node->state = point;
                    break;
                case 'r':
                    node->state = superPoint;
                    break;
                case ' ':
                    node->state = empty;
                    break;
                default:
                    printf("\nError: Invalid character '%c' in map at x: %d, y: %d\n", theLine[x], x, y);
                    free(theLine);
                    free(theMap->graph);
                    fclose(file);
                    return false;
            }
        }
        free(theLine);
    }

    fclose(file);
    return true;
}
int cameraOffSetX(PacMan* pacman,Map* theMap,int screenWidth, int* blockSize){
    if(pacman->position.x>screenWidth/2){
        if( pacman->position.x + screenWidth/2 < theMap->sizeX * *blockSize){
            return -( pacman->position.x - screenWidth/2);
        }
        else{
            return -(theMap->sizeX * *blockSize - screenWidth);
        }
    }
     else return 0;
}
int cameraOffSetY(PacMan* pacman,Map* theMap ,int screenHeigth, int* blockSize){
    if(pacman->position.y>screenHeigth/2){
        if( pacman->position.y + screenHeigth/2 < theMap->sizeY * *blockSize){
            return -( pacman->position.y - screenHeigth/2);
        }
        else{
            return -(theMap->sizeY * *blockSize - screenHeigth);
        }
    }
     else return 0;
}
void renderCurrentMap(SDL_Renderer* renderer,Map* theMap, dynarray* ghosts, PacMan*pacman,SDL_Texture* textures[], int* blockSize, WindowSize* window, int* highscore){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int xOffSet = cameraOffSetX(pacman,theMap,window->width,blockSize);
    int yOffSet = cameraOffSetY(pacman,theMap,window->heigth,blockSize);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //render player
    SDL_Rect rect = {pacman->position.x + *blockSize * 0.135 + xOffSet,pacman->position.y + *blockSize * 0.135 + yOffSet, *blockSize * 0.75, *blockSize * 0.75};
    SDL_RenderCopy(renderer,pacman->texture, NULL, &rect);
    //player behaviour and camera follow as well
    //render ghosts
    for (int i = 0; i < ghosts->size; i++) {
        Ghost* ghost = (Ghost*)ghosts->items[i];
        SDL_Rect rect = {ghost->position.x + *blockSize * 0.135 + xOffSet,ghost->position.y + *blockSize * 0.135 + yOffSet, *blockSize * 0.75, *blockSize * 0.75};
        SDL_RenderCopy(renderer,ghost->texture, NULL, &rect);
    }
    
    //render map
    //everything will be of size 10 for testing purposes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < theMap->sizeX * theMap->sizeY; i++)
    {
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_Rect rect = {theMap->graph[i].position.x * *blockSize, theMap->graph[i].position.y * *blockSize + yOffSet, *blockSize, *blockSize};
        Node* node = &theMap->graph[i];
        //render the map
        switch (theMap->graph[i].state)
        {
        case wall:
            rect.x = theMap->graph[i].position.x * *blockSize + xOffSet;
            rect.y = theMap->graph[i].position.y * *blockSize + yOffSet;
            rect.h = *blockSize;
            rect.w = *blockSize;
            SDL_RenderFillRect(renderer, &rect);
            //render wall
            break;
        case point:
            rect.x = theMap->graph[i].position.x * *blockSize + *blockSize * 0.375 + xOffSet;
            rect.y = theMap->graph[i].position.y * *blockSize + *blockSize * 0.375 + yOffSet;
            rect.h = *blockSize * 0.25;
            rect.w = *blockSize * 0.25;
            //render point
            SDL_RenderCopy(renderer, textures[0], NULL, &rect);
            break;
        case superPoint:
            rect.x = theMap->graph[i].position.x * *blockSize + *blockSize * 0.20 + xOffSet;
            rect.y = theMap->graph[i].position.y * *blockSize + *blockSize * 0.20 + yOffSet;
            rect.h = *blockSize * 0.60;
            rect.w = *blockSize * 0.60;
            //render point
            SDL_RenderCopy(renderer, textures[0], NULL, &rect);
            break;
        case empty:
            //render empty
            break;
        default:
            break;
        }
    }
    TTF_Font* font = TTF_OpenFont("SaniTrixieSans.ttf", 30); 
    SDL_Color textcolor = {255,0,0,0};
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "High Score: %d", *highscore);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textcolor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {(window->width/2 - textSurface->w/2) / 2, window->width*0.01, textSurface->w, textSurface->h}; 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &textRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.x = (window->width/2 - textSurface->w/2) * 1.5;
    snprintf(scoreText, sizeof(scoreText), "Current Score: %d", pacman->score);
    textSurface = TTF_RenderText_Solid(font, scoreText, textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &textRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(textSurface); 
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}
bool isClose(Position* agent, Position* destination,int* blockSize) {
    if (!agent || !destination) {
        printf("failed to find agent or destination\n");
        return false;
    }
    return (abs(agent->x - destination->x * *blockSize) <= 1.5) &&
           (abs(agent->y - destination->y * *blockSize) <= 1.5);
}
bool checkCrossection(Ghost* ghost, Map* theMap){
    switch (ghost->direction)
    {
    case right:
        if(ghost->closeTo->up != NULL){
            if (ghost->closeTo->up->state != wall)
            {
                return true;
            }
        }
        if(ghost->closeTo->down != NULL){
            if (ghost->closeTo->down->state != wall)
            {
                return true;
            }
        }
        return false;
        break;
    case left:
        if(ghost->closeTo->up != NULL){
            if (ghost->closeTo->up->state != wall)
            {
                return true;
            }
        }
        if(ghost->closeTo->down != NULL){
            if (ghost->closeTo->down->state != wall)
            {
                return true;
            }
        }
        return false;
        break;
    case up:
        if(ghost->closeTo->right != NULL){
                if (ghost->closeTo->right->state != wall)
                {
                    return true;
                }
            }
            if(ghost->closeTo->left != NULL){
                if (ghost->closeTo->left->state != wall)
                {
                    return true;
                }
            }
        break;
    case down:
        if(ghost->closeTo->left != NULL){
            if (ghost->closeTo->left->state != wall)
            {
                return true;
            }
        }
        if(ghost->closeTo->right != NULL){
            if (ghost->closeTo->right->state != wall)
            {
                return true;
            }
        }
        break;
    default:
        break;
    }
}
void positionChanges(Map* theMap, dynarray* ghosts, PacMan* pacman, int* blockSize, int channels[]) {
    for (int i = 0; i < ghosts->size; i++) {
        Ghost* ghost = (Ghost*)ghosts->items[i];
        switch (ghost->direction)
        {
        case none:
            int dx = pacman->position.x - ghost->position.x;
            int dy = pacman->position.y - ghost->position.y;

            // Prioritize moving towards the player
            if (abs(dx) > abs(dy)) {
                if (dx > 0) {
                    ghost->direction = right;
                } else {
                    ghost->direction = left;
                }
            } else {
            if (dy > 0) {
                ghost->direction = down;
            } else {
                ghost->direction = up;
            }
            }

            //find new path based on GhostAI
            //ghost->direction = ghostAI(&ghost);
            break;

        case right:
            ghost->movementPercent += ghost->speed;
            if (!ghost->closeTo->right || ghost->closeTo->right->state == wall) {
                ghost->direction = none;
                break;
            }
            if(ghost->movementPercent > 1){
                ghost->position.x += (int)ghost->movementPercent;
                ghost->movementPercent=0;
            }
            if (isClose(&ghost->position, &ghost->closeTo->right->position,blockSize)) {
                ghost->closeTo = ghost->closeTo->right; // Update position node
                ghost->position.x = ghost->closeTo->position.x * *blockSize;
                ghost->position.y = ghost->closeTo->position.y * *blockSize;
                if(checkCrossection(ghost,theMap)) ghost->direction = none;
            }
            break;

        case left:
            ghost->movementPercent += ghost->speed;
            if (!ghost->closeTo->left || ghost->closeTo->left->state == wall) {
                ghost->direction = none;
                break;
            }
            if(ghost->movementPercent > 1){
                ghost->position.x -= (int)ghost->movementPercent;
                ghost->movementPercent=0;
            }
            if (isClose(&ghost->position, &ghost->closeTo->left->position,blockSize)) {
                ghost->closeTo = ghost->closeTo->left; // Update position node
                ghost->position.x = ghost->closeTo->position.x * *blockSize;
                ghost->position.y = ghost->closeTo->position.y * *blockSize;
                if(checkCrossection(ghost,theMap)) ghost->direction = none;
            }
            break;

        case up:
            ghost->movementPercent += ghost->speed;
            if (!ghost->closeTo->up || ghost->closeTo->up->state == wall) {
                ghost->direction = none;
                break;
            }
            if(ghost->movementPercent > 1){
                ghost->position.y -= (int)ghost->movementPercent;
                ghost->movementPercent=0;
            }
            if (isClose(&ghost->position, &ghost->closeTo->up->position,blockSize)) {
                ghost->closeTo = ghost->closeTo->up; // Update position node
                ghost->position.x = ghost->closeTo->position.x * *blockSize;
                ghost->position.y = ghost->closeTo->position.y * *blockSize;
                if(checkCrossection(ghost,theMap)) ghost->direction = none;
            }
            break;

        case down:
            ghost->movementPercent += ghost->speed;
            if (!ghost->closeTo->down || ghost->closeTo->down->state == wall) {
                ghost->direction = none;
                break;
            }
            if(ghost->movementPercent > 1){
                ghost->position.y += (int)ghost->movementPercent;
                ghost->movementPercent=0;
            }
            if (isClose(&ghost->position, &ghost->closeTo->down->position,blockSize)) {
                ghost->closeTo = ghost->closeTo->down; // Update position node
                ghost->position.x = ghost->closeTo->position.x * *blockSize;
                ghost->position.y = ghost->closeTo->position.y * *blockSize;
                if(checkCrossection(ghost,theMap)) ghost->direction = none;
            }
            break;

        default:
            // Invalid direction, do nothing
            break;
        }
    }
    
    switch (pacman->direction) {
        case none:
            //doing nothing
            pacman->movementPercent = 0;
            pacman->direction = pacman->buffer;
            break;

        case right:
            pacman->movementPercent += pacman->speed;
            if (!pacman->closeTo->right || pacman->closeTo->right->state == wall) {
                pacman->direction = none;
                pacman->chomping = false;
                break;
            }
            if(pacman->movementPercent > 1){
                pacman->position.x += (int)pacman->movementPercent;
                pacman->movementPercent=0;
            }
            if (isClose(&pacman->position, &pacman->closeTo->right->position,blockSize)) {
                pacman->closeTo = pacman->closeTo->right; // Update position node
                pacman->position.x = pacman->closeTo->position.x * *blockSize;
                pacman->position.y = pacman->closeTo->position.y * *blockSize;
                pacman->chomping = false;
                pacman->direction = pacman->buffer;
            }
            break;

        case left:
            pacman->movementPercent += pacman->speed;
            if (!pacman->closeTo->left || pacman->closeTo->left->state == wall) {
                pacman->direction = none;
                pacman->chomping = false;
                break;
            }
            if(pacman->movementPercent > 1){
                pacman->position.x -= (int)pacman->movementPercent;
                pacman->movementPercent=0;
            }
            if (isClose(&pacman->position, &pacman->closeTo->left->position,blockSize)) {
                pacman->closeTo = pacman->closeTo->left; // Update position node
                pacman->position.x = pacman->closeTo->position.x * *blockSize;
                pacman->position.y = pacman->closeTo->position.y * *blockSize;
                pacman->chomping = false;
                pacman->direction = pacman->buffer;
            }
            break;

        case up:
            pacman->movementPercent+=pacman->speed;
            if (!pacman->closeTo->up || pacman->closeTo->up->state == wall) {
                pacman->direction = none;
                pacman->chomping = false;
                break;
            }
            if(pacman->movementPercent > 1){
                pacman->position.y -= (int)pacman->movementPercent;
                pacman->movementPercent=0;
            }
            if (isClose(&pacman->position, &pacman->closeTo->up->position,blockSize)) {
                pacman->closeTo = pacman->closeTo->up; // Update position node
                pacman->position.x = pacman->closeTo->position.x * *blockSize;
                pacman->position.y = pacman->closeTo->position.y * *blockSize;
                pacman->chomping = false;
                pacman->direction = pacman->buffer;
            }
            break;

        case down:
            pacman->movementPercent += pacman->speed;
            if (!pacman->closeTo->down || pacman->closeTo->down->state == wall) {
                pacman->direction = none;
                pacman->chomping = false;
                break;
            }
            if(pacman->movementPercent > 1){
                pacman->position.y += (int)pacman->movementPercent;
                pacman->movementPercent=0;
            }
            if (isClose(&pacman->position, &pacman->closeTo->down->position,blockSize)) {
                pacman->closeTo = pacman->closeTo->down; // Update position node
                pacman->position.x = pacman->closeTo->position.x * *blockSize;
                pacman->position.y = pacman->closeTo->position.y * *blockSize;
                pacman->chomping = false;
                pacman->direction = pacman->buffer;
            }
            break;

        default:
            // Invalid direction, do nothing
            break;
    }
}
float distance(Position* p1, Position* p2) {
    float dx = p2->x - p1->x;
    float dy = p2->y - p1->y;
    return sqrt(dx * dx + dy * dy);
}
int checkIfGhosted(dynarray* ghosts,PacMan* pacman,int* blockSize){
    for (size_t i = 0; i < ghosts->size; i++)
    {
        Ghost* ghost = ghosts->items[i];
        if(distance(&pacman->position,&ghost->position) <= 1){
            return i;
        }
    }
    return 0;
}
void killGhost(int* id,dynarray* ghosts){
    Ghost* ghost = ghosts->items[*id];
    ghost->position = ghost->originPosition;
    ghost->closeTo = ghost->originNode; 
}
void killPlayer(PacMan* pacman, dynarray* ghosts){
    for (size_t i = 0; i < ghosts->size; i++)
    {
        Ghost* ghost = ghosts->items[i];
        ghost->position = ghost->originPosition;
        ghost->closeTo = ghost->originNode;
    }
    pacman->lives--;
    if(pacman->lives > 0){
        pacman->closeTo = pacman->originalNode;
        pacman->position = pacman->originalPosition;
    }
    else{
        pacman->dead = true;
    }
    
}
void playerInteraction(Map* theMap, dynarray* ghost, PacMan* pacman, int* blockSize){
    //taking normal point
    int id;
    if(pacman->closeTo->state == point){
        pacman->chomping = true;
        pacman->closeTo->state = empty;
        pacman->score += 1;
    }
    //taking superpoint
    if(pacman->dangerous){
        if(id = checkIfGhosted(ghost,pacman,blockSize)){
            killGhost(&id, ghost);
            pacman->score += 10;
        }
        if(pacman->dangerousTimer + 10000 < SDL_GetTicks()){
            pacman->dangerous = false;
        }
    }
    if(id = checkIfGhosted(ghost,pacman,blockSize)){
        killPlayer(pacman,ghost);
    }
    if(pacman->closeTo->state == superPoint){
        pacman->dangerousTimer = SDL_GetTicks();
        pacman->chomping = true;
        pacman->dangerous = true;
        pacman->closeTo->state = empty;
        pacman->score += 2;
    }

}
void drawMenu(SDL_Renderer *renderer, int highScore) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect startGame = {100, 100, 200, 50};
    SDL_Rect exit = {100, 200, 200, 50};
    SDL_Rect setResolution = {100, 300, 200, 50};

    SDL_RenderFillRect(renderer, &startGame);
    SDL_RenderFillRect(renderer, &exit);
    SDL_RenderFillRect(renderer, &setResolution);

    TTF_Font* font = TTF_OpenFont("SaniTrixieSans.ttf", 30); 
    SDL_Color textcolor = {255,255,255,255};
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "High Score: %d", highScore);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textcolor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {100, 500, textSurface->w, textSurface->h}; 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); 
    textcolor.r = 0;
    textcolor.g = 0;
    textcolor.b = 0;
    textSurface = TTF_RenderText_Solid(font, "Start", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = 105;
    textRect.y = 105; 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textSurface = TTF_RenderText_Solid(font, "Exit", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = 105;
    textRect.y = 205; 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textSurface = TTF_RenderText_Solid(font, "Options", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = 105;
    textRect.y = 305; 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface); 
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
}
void drawResolutionOptions(SDL_Renderer *renderer, char* difficulty) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Resolution option rectangles
    SDL_Rect res1080p = {100, 100, 200, 50};
    SDL_Rect res720p = {100, 200, 200, 50};
    SDL_Rect res600p = {100, 300, 200, 50};
    SDL_Rect fullscreen = {100, 400, 200, 50};

    // Additional buttons
    SDL_Rect backButton = {400, 100, 200, 50};    // Right-side button
    SDL_Rect difficultyButton = {400, 200, 200, 50}; // Right-side button

    SDL_RenderFillRect(renderer, &res1080p);
    SDL_RenderFillRect(renderer, &res720p);
    SDL_RenderFillRect(renderer, &res600p);
    SDL_RenderFillRect(renderer, &fullscreen);
    SDL_RenderFillRect(renderer, &backButton);
    SDL_RenderFillRect(renderer, &difficultyButton);

    TTF_Font* font = TTF_OpenFont("SaniTrixieSans.ttf", 30); 
    SDL_Color textcolor = {0,0,0,0};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "1920/1080", textcolor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {100, 100, textSurface->w, textSurface->h}; 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.y += 100;
    textSurface = TTF_RenderText_Solid(font, "1280/720", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.y += 100;
    textSurface = TTF_RenderText_Solid(font, "800/600", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.y += 100;
    textSurface = TTF_RenderText_Solid(font, "Fullscreen", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.y = 100;
    textRect.x = 400;
    textSurface = TTF_RenderText_Solid(font, "Return", textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    textRect.y = 200;
    textRect.x = 400;
    textSurface = TTF_RenderText_Solid(font, difficulty, textcolor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface); 
    SDL_RenderPresent(renderer);
}
int isPointInsideRect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= (rect->x + rect->w) &&
           y >= rect->y && y <= (rect->y + rect->h);
}
int readHighScore(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("High score file not found. Assuming high score is 0.\n");
        return 0;
    }

    int highScore = 0;
    if (fscanf(file, "%d", &highScore) != 1) {
        printf("Failed to read high score. Assuming high score is 0.\n");
        highScore = 0;
    }

    fclose(file);
    return highScore;
}
void writeHighScore(const char* filename, int newScore) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open high score file for writing.\n");
        return;
    }
    fprintf(file, "%d", newScore);
    fclose(file);
}

void correctScore(int* highscore,int* score){
    if(highscore < score){
        writeHighScore("highscore.txt",*score);
    }
}
int main(int argc, char *argv[]) {
    // Nastavení environmentálních proměnných
    setenv("SDL_VIDEODRIVER", "x11", 1);
    setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    int channels[3];
    SDL_Texture* textures[2];
    Map theMap;
    dynarray ghosts;
    dynarray_init(&ghosts, 4);
    PacMan pacman;
    WindowSize windowsize;
    windowsize.heigth = 600;
    windowsize.width = 800;
    int setDifficulty = 0;
    char difficulty[5][50] = {
        "easy",
        "normal",
        "hard"
    };
    int blockSize = 100;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Chyba inicializace SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
    printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
    return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowsize.width, windowsize.heigth, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Chyba při vytváření okna: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Použití softwarového rendereru
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("Chyba při vytváření rendereru: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!makeMap(&theMap,&ghosts,&pacman,&blockSize,renderer)){
        printf("failed mapmaking \n");
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
    printf("SDL_image initialization failed: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
    }
    textures[0] = IMG_LoadTexture(renderer, "yellowCircle.jpg");
    if (!textures[0]) {
        printf("Failed to load point texture: %s\n", IMG_GetError());
    }
    Mix_Chunk* chompSound = Mix_LoadWAV("Chomp.wav");
    int chomping = Mix_PlayChannel(-1,chompSound,-1);
    Mix_Pause(chomping);
    int running = 1;
    pacman.texture = IMG_LoadTexture(renderer, "pacman.png");
    SDL_Event event;
    SDL_Rect startGame = {100, 100, 200, 50};
    SDL_Rect exit = {100, 200, 200, 50};
    SDL_Rect setResolution = {100, 300, 200, 50};
    GameState currentMenu = Main_Menu;
    bool fullscreen = false;
    pacman.direction = left;
    bool chompingStopped = false;
    int highScore = readHighScore("highscore.txt");
    while (running) {
        if (currentMenu == Main_Menu) {
            drawMenu(renderer,highScore);
        } else if (currentMenu == Options_Menu) {
            drawResolutionOptions(renderer,difficulty[setDifficulty]);
        }
        else if (currentMenu == Game)
        {
            if(pacman.chomping){
                Mix_Resume(chomping); chompingStopped = false;
            } else if(!chompingStopped) { Mix_Pause(chomping); Mix_PlayChannel(-1,chompSound,0); chompingStopped = true;}
            renderCurrentMap(renderer,&theMap,&ghosts,&pacman,textures,&blockSize,&windowsize,&highScore);
            positionChanges(&theMap,&ghosts,&pacman,&blockSize,channels);
            playerInteraction(&theMap,&ghosts,&pacman,&blockSize);
            //updateGhostGroups(&allGhosts);
            //switchGhostStates(&allGhosts, gameLevel);
            // Process ghost behaviors
        }
        
        while (SDL_PollEvent(&event)) {
            int x = event.button.x;
            int y = event.button.y;
            switch (currentMenu)
            {
            case Main_Menu:
                if (event.type == SDL_QUIT) {
                running = 0;
                correctScore(&highScore,&pacman.score);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {

                if (isPointInsideRect(x, y, &startGame)) {
                    printf("Start Game button clicked!\n");
                    currentMenu = Game;
                    
                } else if (isPointInsideRect(x, y, &exit)) {
                    printf("Load File button clicked!\n");
                    running = 0;
                    correctScore(&highScore,&pacman.score);
                    // Handle file loading logic here
                } else if (isPointInsideRect(x, y, &setResolution)) {
                    printf("Set Resolution button clicked!\n");
                    currentMenu = Options_Menu;
                }
            }
                break;
            case Options_Menu:
            if (event.type == SDL_QUIT) {
                running = 0;
                correctScore(&highScore,&pacman.score);
            }else if(event.type == SDL_MOUSEBUTTONDOWN){
                 if (x >= 100 && x <= 300) {
                        if (y >= 100 && y <= 150) {
                            windowsize.width = 1920;
                            windowsize.heigth = 1080;
                            SDL_SetWindowSize(window, windowsize.width, windowsize.heigth);
                            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                        } else if (y >= 200 && y <= 250) {
                            windowsize.width = 1280;
                            windowsize.heigth = 720;
                            SDL_SetWindowSize(window, windowsize.width, windowsize.heigth);
                            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                        } else if (y >= 300 && y <= 350) {
                            windowsize.width = 800;
                            windowsize.heigth = 600;
                            SDL_SetWindowSize(window, windowsize.width, windowsize.heigth);
                            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                        } else if (y >= 400 && y <= 450) {
                            if(!fullscreen){
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                                SDL_GetWindowSize(window, &windowsize.width, &windowsize.heigth);
                                fullscreen = true;
                            }
                            else{
                                SDL_SetWindowFullscreen(window, 0);
                                windowsize.width = 800;
                                windowsize.heigth = 600;
                                SDL_SetWindowSize(window, windowsize.width, windowsize.heigth);
                                SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                                fullscreen = false;
                            }
                        }
                    }
                    if (x >= 400 && x <= 600) {
                        if (y >= 100 && y <= 150) {
                            currentMenu = Main_Menu; 
                        } else if (y >= 200 && y <= 250) {
                            printf("Change Difficulty clicked\n");
                            if(setDifficulty < 2) setDifficulty++;
                            else setDifficulty = 0;
                        }
                    }
            }
                break;
            case Game:
            if (event.type == SDL_QUIT || pacman.dead) {
                running = 0;
                correctScore(&highScore,&pacman.score);
                }
            else if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym)
                {
                case SDLK_d:
                    pacman.buffer = right;
                    break;
                case SDLK_a:
                    pacman.buffer = left;
                    break;
                case SDLK_s:
                    pacman.buffer = down;
                    break;
                case SDLK_w:
                    pacman.buffer = up;
                    break;
                case SDLK_ESCAPE:
                    currentMenu = Main_Menu;
                    break;
                
                default:
                    break;
                }
            }
                break;
            default:
                break;
            }
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
