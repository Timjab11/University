#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int checkAmountOfDigits(int number) {
	if (number == 0) { 
		return 1;
	}
	int digits = 1;
	while (true)
	{
		if (number > 9) {
			number /= 10;
			digits++;
			continue;
		}
		return digits;
	}
}
void drawHorizontal(int* numbers, int* invalid, int* minNumber) {
	int max = checkAmountOfDigits(*minNumber+8);
	for (int i = 0; i <= 8; i++)
	{
		//prints the numbers and how many there are
		//because there is only chance of getting one more digit more than the previous number
		//there will alway be upmost 1 more space
		if (max > checkAmountOfDigits(*minNumber + i)) {
			printf(" ");
		}
		printf("%d", *minNumber + i);
		if (*(numbers + i) > 0) {
			printf(" ");
			for (int x = 0; x < *(numbers + i); x++)
			{
				printf("#");
			}
		}
		printf("\n");
	}
	if (*invalid > 0) {
		//prints invalid and how many there are
		printf("invalid: ");
		for (int i = 0; i < *invalid; i++)
		{
			printf("#");
		}
		printf("\n");
	}
}
void drawVertical(int* numbers, int* invalid, int* minNumber, int* max) {
	//makes sure the max is real and not fictional
	if (*max < *invalid) {
		*max = *invalid;
	}
	for (int i = 0; i < *max; i++)
	{
		//print invalid vertical line
		if (*invalid >= *max - i) {
			printf("#");
		}
		else {
			printf(" ");
		}
		//print out lines for numbers
		//from min to min+8
		for (int xp = 0; xp <= 8; xp++)
		{
			if (*(numbers + xp) >= *max - i) {
				printf("#");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("i");
	for (int i = 0; i <= 8; i++)
	{
		printf("%d", *minNumber + i);
	}
	printf("\n");

}
bool drawHistogram() {
	char input[3];
	scanf("%2s", input);
	if ((strlen(input) == 1 && input[0] == 'v' )|| input[0] == 'h') {
		int inputSize;
		int minNumber;
		int noUse = 0;
		int max = 0;
		scanf("%d%d", &inputSize, &minNumber);
		int foundNumbers[9] = { 0 };
		int number;
		for (int i = 0; i < inputSize; i++)
		{
			scanf("%d", &number);
			if (number >= minNumber && number <= minNumber + 8) {
				*(foundNumbers + number - minNumber) += 1; //tohle je docela cool způsob zápisu, který nevím, jestli je praktický, ale ještě jsem ho neviděl :D
				if (max < *(foundNumbers + number - minNumber)) {
					max = *(foundNumbers + number - minNumber);
				}
			}
			else {
				noUse += 1;
			}
		}
		if (*input == 'h') {
			drawHorizontal(&foundNumbers, &noUse, &minNumber);
			return 0;
		}
		drawVertical(&foundNumbers, &noUse, &minNumber, &max);
		return 0;
	}
	else {
		printf("Neplatny mod vykresleni\n");
		return 1;
	}
	return 0;
}
int main() {
	return drawHistogram();
}