#include <stdio.h>

#define PI 3.14
#define EXIT_MSG "Exiting...\n"

int main(int argc, char *argv[]) {
	printf("Enter images directory path:\n");
	fflush(NULL);
	printf("Enter images prefix:\n");
	fflush(NULL);
	printf("Enter number of images:\n");
	fflush(NULL);
		printf("An error occurred - invalid number of images\n");
	fflush(NULL);
	printf("Enter images suffix:\n");
	fflush(NULL);
	printf("Enter number of bins:\n");
	fflush(NULL);
		printf("An error occurred - invalid number of bins\n");
	fflush(NULL);
	printf("Enter number of features:\n");
	fflush(NULL);
		printf("An error occurred - invalid number of features\n");
	fflush(NULL);
	printf("Enter a query image or # to terminate:\n");
	fflush(NULL);
	printf(EXIT_MSG);
	fflush(NULL);

	printf("Nearest images using global descriptors:\n");
	fflush(NULL);
	printf("i1, i2, i3, i4, i5\n");
	fflush(NULL);
	printf("Nearest images using local descriptors:\n");
	fflush(NULL);
	printf("j1, j2, j3, j4, j5\n");
	fflush(NULL);
		printf("An error occurred - allocation failure\n");
	fflush(NULL);
	return 0;
}


