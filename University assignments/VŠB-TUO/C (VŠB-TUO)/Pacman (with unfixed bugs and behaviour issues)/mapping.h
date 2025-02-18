#pragma once
#include "localizer.h"
typedef enum{
    empty,
    point,
    superPoint,
    wall
} NodeState;
typedef struct Node Node;
typedef struct Node
{
    Position position;
    Node* left;
    Node* right;
    Node* up;
    Node* down;
    NodeState state;
} Node;
typedef struct 
{
    int sizeX,sizeY;
    Node* graph;
} Map;

