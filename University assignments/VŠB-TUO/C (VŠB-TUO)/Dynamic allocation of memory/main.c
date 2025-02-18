#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int createTurtle(int* turtles, int* amount ){
    if(*amount >= 2){
        return turtles;
    }
    (*amount)++;
    // {position x, position y, smer}
    *(turtles + (*amount * 3)) = 0;
    *(turtles + (*amount * 3) + 1) = 0;
    *(turtles + (*amount * 3) + 2) = 1;
    return turtles;
}
char* createField(int *width, int *height){
    int size = *width * *height;
    char* array = (char*)malloc(*width * *height * sizeof(char));
    for (int i = 0; i < size; i++)
    {
        *(array + i) = '.';
    }
    return array;
}
bool checkIfTurtle(int* turtles, int* x, int* y,int* amount){
    for (int i = 0; i <= *amount; i++)
    {
        if( *(turtles + (i * 3)) == *x && *(turtles + 1 + (i * 3)) == *y){
            return true;
        }
    }
    return false;
}
void drawTheField(int* turtles,int *width,int* heigth, char* array, int* amount){ //draws the field out 
    int position = 0;
    for (int i = 0; i < *width * *heigth; i++)
    {
        if(position == *width){
            printf("\n");
            position = 0;
        }
        int x = i % *width;
        int y = i / *width;
        int size = *amount;
       /* if(checkIfTurtle(turtles,&x, &y, &size)){ // for checking where it is
            printf("z");
        }
        else{*/
            printf("%c",*(array + i));
        //}
        position++;
    }
}
void rotateRight(int* turtles,int* amount){
    for (int i = 0; i <= (*amount); i++)
    {
        *(turtles + (i * 3)+2) = (*(turtles + (i * 3)+2) + 1) % 4;
    }
    
}
void rotateLeft(int* turtles, int* amount){
    for (int i = 0; i <= (*amount); i++)
    {
        *(turtles + (i * 3)+2) = (*(turtles + (i * 3)+2) - 1);
        if(*(turtles + (i * 3)+2) < 0){
            *(turtles + (i * 3)+2) = 3;
        }
    }
}
void moveTurtles(int* turtles, int* amount, int* height, int* width){
    for (int i = 0; i <= (*amount); i++)
    {
        switch (*(turtles + (i * 3) + 2))
        {
        case 0: //up
            *(turtles + (i * 3)+1) = (*(turtles + (i * 3)+1) - 1) % *height;
            if(*(turtles + (i * 3)+1) < 0){
                *(turtles + (i * 3)+1) = *height - 1;
            }
            break;
        case 1: //right
            *(turtles + (i * 3)) = (*(turtles + (i * 3)) + 1) % *width;
            break;
        case 2: //down
            *(turtles + (i * 3)+1) = (*(turtles + (i * 3)+1) +1) % *height;
            break;
        case 3: //left
            *(turtles + (i * 3)) = (*(turtles + (i * 3)) -1) % *width;
            if(*(turtles + (i * 3)) < 0){
                *(turtles + (i * 3)) = *width - 1;
            }
            break;
        default:
            printf("\n Unwanted result \n");
            break;
        }
    }
}
void turtlePrint(int* turtles, char* array, int* amount,int* width){
    for (int i = 0; i <= (*amount); i++)
    {
        if(*(array + *(turtles + (i * 3)) + (*(turtles + (i * 3)+1) * *width)) == 'o'){
            *(array + *(turtles + (i * 3)) + (*(turtles + (i * 3)+1) * *width)) = '.';
            continue;
        }
        *(array + *(turtles + (i * 3)) + (*(turtles + (i * 3)+1) * *width))='o';

    }
    
}
void commandTurtles(char* command, int* turtles, int *width, int* height, int* amount, char* array){
    int size = *amount;
    int climb = *height;
    int side = *width;
    switch (*command)
    {
    case 'r':
        rotateRight(turtles,&size);
        break;
    case 'l':
        rotateLeft(turtles,&size);
        break;
    case'm':
        moveTurtles(turtles,&size,&climb,&side);
        break;
    case 'x':
        //terminate
        break;
    case 'o':
        turtlePrint(turtles,array,&size,&side);
        break;
    case 'f':
        if(*amount < 2){
            createTurtle(turtles,&size);
            (*amount)++;    
        }
        break;
    default:
        break;
    }

}
int main() {
	int width;
    int height;
    int amount = -1;
    scanf("%d%d",&height,&width);
    char* array = createField(&width,&height); //for creating field out of width and height
    int* turtles = (int*)malloc(9 * sizeof(int)); //jen abych se ujistil že to projde, příště to vytvořím jako normální int[]
    createTurtle(turtles,&amount);
    char c = 'p';
        while(c != 'x'){
        scanf("%c",&c);
        scanf("%c",&c);
        commandTurtles(&c,turtles,&width,&height,&amount,array);
        //drawTheField(turtles,&width, &height, array,&amount);//for checking where it is
    }
    drawTheField(turtles,&width, &height, array,&amount);
    printf("\n");
    free(array);
    free(turtles);
    return 0;
}