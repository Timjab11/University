#include "drawing.h"
#include <stdbool.h>
#include <stdlib.h>
void moveLeft(int spaces){
    for (int i = 0; i < spaces; i++)
    {
      move_left();
    }
}
void moveRight(int spaces){
    for (int i = 0; i < spaces; i++)
    {
      move_right();
    }
}
void moveDown(int spaces){
    for (int i = 0; i < spaces; i++)
    {
      move_down();
    }
}
void moveUp(int spaces){
    for (int i = 0; i < spaces; i++)
    {
      move_up();
    }
}
void changeColor(int color){
  switch (color)
  {
  case 0:
    set_red_color();
    break;
  case 1:
    set_green_color();
    break;
  case 2:
    set_blue_color();
    break;
  case 3:
    set_white_color();
    break;
  case 4:
    set_yellow_color();
    break;
  case 5:
    set_black_color();
  default:
    printf("this color doesnt exist");
    break;
  }  
}
void drawLine(int length, int color, int min_length, int max_length){
  int rightmoves = 0;
  if(min_length > max_length){
    printf("max length is smaller than min length");
    return;
  }
  bool randomColor = false;
  if(color > 4){
    randomColor = true;
    changeColor(rand()%5);
  }
  else{
    changeColor(color);
  }
  draw_pixel();
  for (int i = 1; i < length; i++)
  {
    if(randomColor){
      changeColor(rand()%5);
    }
    move_right();
    rightmoves++;
    int spaceLength = min_length + rand() % (max_length+1 - min_length);
    for (int i = 0; i < spaceLength; i++)
    {
      move_right();
      rightmoves++;
    }
    draw_pixel();
  }
  //return to Origin
  moveLeft(rightmoves);
}
void drawStairs(int amount, int height, int width, int color){
    int rightmoves = 0;
    int downmoves = 0;
    void drawStair(int height, int width, int color){
      changeColor(color);
      draw_pixel();
      for (int i = 0; i < width; i++)
      {
        move_right();
        rightmoves++;
        draw_pixel();
      }
      move_down();
      downmoves++;
      for (int i = 0; i < height; i++)
      {
        draw_pixel();
        move_down();
        downmoves++;
      }
    }
    for (int i = 0; i < amount; i++)
    {
      if(color > 5){
        drawStair(height,width,rand()%5);
      }
      else{
        drawStair(height,width,color);
      }
    }
    //return to origin
    moveLeft(rightmoves);
    moveUp(downmoves);
}
void drawFlowers(int bodyLength, int armLength,int height, int bodyColor, int armColor){
    void drawBody(){
      changeColor(bodyColor);
      for (int i = 0; i < bodyLength; i++)
      {
        for (int i = 0; i < bodyLength; i++)
        {
          draw_pixel();
          move_right();
        }
        moveLeft(bodyLength);
        move_down();
      }
      moveUp(bodyLength);
      
    }
    void drawArms(){
      changeColor(armColor);
      //ALWAYS RETURN TO POINT OF ORIGIN
      //left arm
      for (int i = 0; i < bodyLength; i++)
      {
        for (int i = 0; i < armLength; i++)
        {
          move_left();
          draw_pixel();
        }
        moveRight(armLength);
        move_down();
      }
      //return to origin
      moveUp(bodyLength);
      //bottom arm
      moveDown(bodyLength-1);
      for (int i = 0; i < bodyLength; i++)
      {
        for (int i = 0; i < armLength; i++)
        {
          move_down();
          draw_pixel();
        }
        moveUp(armLength);
        move_right();
      }
      //return to origin
      moveLeft(bodyLength);
      moveUp(bodyLength-1);
      //right arm
      moveRight(bodyLength-1);
      for (int i = 0; i < bodyLength; i++)
      {
        for (int i = 0; i < armLength; i++)
        {
          move_right();
          draw_pixel();
        }
        moveLeft(armLength);
        move_down();
      }
      //return to origin
      moveUp(bodyLength);
      moveLeft(bodyLength-1);
      //top arm
      for (int i = 0; i < bodyLength; i++)
      {
        for (int i = 0; i < armLength; i++)
        {
          move_up();
          draw_pixel();
        }
        moveDown(armLength);
        move_right();
      }
      moveLeft(bodyLength);
    }
    void drawStonek(){ //hihi
      changeColor(1);
      for (int i = 0; i < height; i++)
      {
        draw_pixel();
        move_down();
      }
      moveUp(height);
    }
    void drawFlower(){
      drawStonek();
      drawBody();
      drawArms();
    }
    drawFlower();
}
void drawField(int rows,int columns){
  int rightMove = 0;
  int downMove = 0;
  moveRight(3);
  rightMove += 3;
  moveDown(3);
  downMove += 3;
  for (int i = 0; i < rows; i++)
  {
    for (int i = 0; i < columns; i++)
    {
        drawFlowers(1,1,3,3,4);
        moveRight(4);
        rightMove += 4;
    }
    moveLeft(columns * 4);
    rightMove += columns * 4;
    moveDown(5);
    downMove += 5;
  }
  moveLeft(rightMove);
  moveUp(downMove);
}
void animation_of_T(){
  void drawBody(){
  //set starting position
  moveRight(10);
  moveDown(3);
  //draw main body
  set_green_color();
  draw_pixel();
  //draw connectors
  set_yellow_color();
  move_down();
  draw_pixel();
  move_up();
  move_left();
  draw_pixel();
  moveRight(2);
  draw_pixel();
  //return to point of origin
  move_left();
  }
  void drawArm(int parts,bool descending){
    //parts is misleading it is one part of length
    //length is 4
    //left arm
    set_green_color();
    int drawn = 0;
    moveLeft(2);
    if(parts == 1){
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        move_left();
        move_up();
      }
      moveRight(4);
      moveDown(4);
    }
    else if(descending){
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        drawn++;
        if(drawn == parts){
          drawn = 0;
          move_left();
        }
        else{
          move_up();
        }
      }
      moveDown(parts);
      moveRight(4 - parts);
      if(parts == 4){
        move_right();
        move_up();
      }
    }
    else{
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        drawn++;
        if(drawn == parts){
          drawn = 0;
          move_up();
        }
        else{
          move_left();
        }
      }
      moveRight(parts);
      moveDown(4 - parts);
      //quick sphagetty code fix
      if(parts == 4){
        move_down();
      }
    }
    moveRight(4);
     if(parts == 1){
      move_up();
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        move_right();
        move_up();
      }
      moveLeft(4);
      moveDown(4);
    }
    else if(descending){
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        drawn++;
        if(drawn == parts){
          drawn = 0;
          move_right();
        }
        else{
          move_up();
        }
      }
      moveDown(parts);
      moveLeft(4 - parts);
      if(parts == 4){
        move_up();
      }
    }
    else{
      //sphagetti code solution
      if(parts == 4){
        move_left();
      }
      for (int i = 0; i < 4; i++)
      {
        draw_pixel();
        drawn++;
        if(drawn == parts){
          drawn = 0;
          move_up();
        }
        else{
          move_right();
        }
      }
      moveLeft(parts);
      moveDown(4 - parts);
      //quick sphagetty code fix
      if(parts == 4){
        move_down();
      }
      }
    //from point of origin paint right arm
    moveLeft(2);
    if(descending){
      if(parts == 4){
        move_left();
      }
    }
    else{
      if(parts == 4){
        move_right();
      }
    }
  }
  void drawLeg(int sequence){
    if(sequence >= 5){
      sequence = 5;
    }
    void drawRightLine(int x){
      for (int i = 0; i < x; i++)
      {
        draw_pixel();
        move_right();
      }
      moveLeft(x);
    }
    void drawLeftLine(int x){
      for (int i = 0; i < x; i++)
      {
        draw_pixel();
        move_left();
      }
      moveRight(x);
    }
    //grows from 7
    moveDown(7);
    draw_pixel();
    //Věřím že to jde i jinak s kratším kódem, ale nechce se mi řešit tedkom pole
    move_left();
    drawLeftLine(5-sequence);
    moveRight(2);
    drawRightLine(5-sequence);
    move_left();
    for (int i = 0; i < sequence; i++)
    {
      draw_pixel();
      move_up();
    }
    draw_pixel();
    moveUp(5-sequence);
  }
  void drawOtherLetters(int sequence){
    set_white_color();
    moveRight(7);
    moveUp(4);
    void drawI(int sequence){
      if(sequence > 7){
        sequence = 7;
      }
      moveDown(sequence);
      for (int i = 0; i < 4; i++)
      {
        if(sequence != 7 || i != 1){
          draw_pixel();
          move_down();
        }
      }
    }
  drawI(sequence);
  }
  int parts = 4;
  int sequence = 0;
  bool descending = true;
  for (int i = 0; i < 10; i++)
  {
    drawBody();
    drawArm(parts,descending);
    if(descending){
      parts /= 2;
      if(parts == 1){ descending = false; }
    }
    else if(parts != 4){
      descending = false;
      parts *=2;
    }
    drawLeg(sequence);
    drawOtherLetters(sequence);
    sequence++;
    animate();
    clear_screen();
  }
}
void drawPainting(int i){
  switch (i)
  {
  case 0:
    drawLine(5,1,1,1);
    printf("\n");
    moveRight(3);
    drawLine(6,2,1,1);
    printf("\n");
    //from this point onwards the spaces are randomized between min and max
    moveRight(5);
    drawLine(7,3,1,4);
    printf("\n");
    moveRight(1);
    drawLine(7,10,1,4);
    break;
  case 1:
    drawStairs(3,2,2,1);
    moveRight(10);
    drawStairs(3,1,1,2);
    moveRight(10);
    drawStairs(4,2,4,3);
    moveRight(15);
    drawStairs(4,0,2,4);
    break;
  case 2:
    moveDown(3);
    moveRight(3);
    drawFlowers(3,2,8,3,4);
    moveRight(10);
    drawFlowers(5,2,8,3,4);
    moveRight(10);
    drawFlowers(1,1,5,3,4);
    moveRight(10);
    drawFlowers(2,2,6,3,4);
    break;
  case 3:
    drawField(2,5);
    break;
  case 4:
  /* Dodělám zítra jen chci zatím hlavně vědět že jsou ostatní tasks správně udělané */
    animation_of_T();
    break;
  default:
    break;
  }
}
int main() {
  // Keep this line here
  clear_screen();

  // Load the input - what should be drawn.
  int drawing = 0;
  scanf("%d", &drawing);

  // Put your drawing code here
  drawPainting(drawing);
  // Keep this line here
  end_drawing();

  return 0;
}