#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv) {
	bool di = 0;
	bool o = 0;
	bool input = 0;
	bool output = 0;
	bool needle = 0;
	char* needleStr = NULL;
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;
	if (argc < 2) {
		printf("Input path not provided\n");
		return 1;
	}
	if (argc < 3) {
		printf("Needle not provided\n");
		return 1;
	}
	for (size_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o")==0) {
			if (i+1 == argc) {
				printf("Missing output path\n");
				return 1;
			}
			if (o) {
				printf("Parameter -o provided multiple times\n");
				return 1;
			}
			outputFile = fopen(argv[i+1],"w");
			i++;
			o = true;

		}
		else if (strcmp(argv[i],"-i")==0) {
			if (di) {
				printf("Parameter -i provided multiple times\n");
				return 1;
			}
			di = true;
		}
		else if (!input) {
			inputFile = fopen(argv[i], "r");
			input = true;
		}
		else if (!needle) {
			needle = true;
			needleStr =  malloc(sizeof(char) * strlen(argv[i]));
			needleStr = argv[i];
		}
		else {
			printf("Too many parameters provided\n");
			return 1;
		}
	}
	if (!input) {
		printf("Input path not provided\n");
		return 1;
	}
	if (!needle) {
		printf("Needle not provided\n");
		return 1;
	}
	if (inputFile == NULL) {
		printf("Input path not provided\n");
		return 1;
	}
	//read through
	char line[101] = "";
	while (fgets(line,100,inputFile)!=NULL)
	{
		size_t a = 0;
		size_t b = 0;
		for (size_t i = 0; i < strlen(line); i++)
		{
			if (di == true){
				if (tolower(needleStr[a]) == tolower(line[b])) {
					a++;
					b++;
					if (a >= strlen(needleStr)) {
						if (a == 1) {
							if (!o) {
								printf(line);
							}
							else {
								fprintf(outputFile,line);
								//write in output
							}
							
							a = 0;
							break;
						}
						else {
							if (!o) {
								printf(line);
							}
							else {
								fprintf(outputFile,line);
								//write in output
							}
							a = 0;
							break;
						}
						a = 0;
					}
				}
				else
				{
					if (a != 0) {
						i--;
					}
					else { b++; }
					a = 0;
				}
		}
			else
			{
				if (needleStr[a]==line[b]) {
					a++;
					b++;
					if (a >= strlen(needleStr)) {
						if (a == 1) {
							if (!o) {
								printf(line);
							}
							else {
								fprintf(outputFile, line);
								//write in output
							}
							a = 0;
							break;
						}
						else {
							if (!o) {
								printf(line);
							}
							else {
								fprintf(outputFile, line);
								//write in output
							}
							a = 0;
							break;
						}
						a = 0;
					}
				}
				else
				{
					if (a != 0) {
						i--;
					}
					else { b++; }
					a = 0;
				}
			}
			}
	}
	fclose(inputFile);
	if (outputFile != NULL) {
		fclose(outputFile);
	}
}