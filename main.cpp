#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdio.h>

#define PI 3.14
#define IMGS_DIR_MSG "Enter images directory path:\n"
#define IMGS_PREFIX_MSG "Enter images prefix:\n"
#define NUM_IMGS_MSG "Enter number of images:\n"
#define IMGS_SUFFIX_MSG "Enter images suffix:\n"
#define NUM_BINS_MSG "Enter number of bins:\n"
#define NUM_FEATURES_MSG "Enter number of features:\n"
#define QUERY_IMG_MSG "Enter a query image or # to terminate:\n"
#define NEAREST_GLOBAL_MSG "Nearest images using global descriptors:\n"
#define NEAREST_LOCAL_MSG "Nearest images using local descriptors:\n"
#define ERROR_NUM_IMGS_MSG "An error occurred - invalid number of images\n"
#define ERROR_NUM_BINS_MSG "An error occurred - invalid number of bins\n"
#define ERROR_NUM_FEATURES_MSG "An error occurred - invalid number of features\n"
#define EXIT_MSG "Exiting...\n"
#define ERROR_ALLOCATION_MSG "An error occurred - allocation failure\n"

int main(int argc, char *argv[]) {
	// Input variable
	int numberOfImages,nBins,maxNFeatures;
	char dir[1025],prefix[1025],suffix[1025],queryImage[1025];
	// Program variable
	int closestHist[5],closestSift[5]; // Five closest images
	int ***arrayHist; // arrayHist = [Image number][R/G/B][nBins]
	double ***arraySift; // arraySift = [Image number][nFeatures][128]
	// Input
	printf(IMGS_DIR_MSG);
	fflush(stdout);
	scanf("%1024s",dir);
	printf(IMGS_PREFIX_MSG);
	fflush(stdout);
	scanf("%1024s",prefix);
	printf(NUM_IMGS_MSG);
	fflush(stdout);
	scanf("%d",&numberOfImages);
	if (numberOfImages<1) {
		printf(ERROR_NUM_IMGS_MSG);
		fflush(stdout);
		//TODO free memory
	}
	printf(IMGS_SUFFIX_MSG);
	fflush(stdout);
	scanf("%1024s",suffix);
	printf(NUM_BINS_MSG);
	fflush(stdout);
	scanf("%d",&nBins);
	if (nBins<1) {
		printf(ERROR_NUM_BINS_MSG);
		fflush(stdout);
		//TODO free memory
	}
	printf(NUM_FEATURES_MSG);
	fflush(stdout);
	scanf("%d",&maxNFeatures);
	if (maxNFeatures<1) {
		printf(ERROR_NUM_FEATURES_MSG);
		fflush(stdout);
		//TODO free memory
	}
	// Allocate memory
	arrayHist = (int ***)malloc(numberOfImages * sizeof(int**));
	arraySift = (double ***)malloc(numberOfImages * sizeof(double**));
	if ((arrayHist == NULL)or(arraySift == NULL)) {
		printf(ERROR_ALLOCATION_MSG);
		fflush(NULL);
		//TODO free memory
		//TODO terminate the program
	}
	for (int i=0;i<numberOfImages;i++) {
		arrayHist[i] = (int **)malloc(3 * sizeof(int*));
		arraySift[i] = (double **)malloc(maxNFeatures * sizeof(double*));
		if ((arrayHist[i] == NULL)or(arraySift[i] == NULL)) {
			printf(ERROR_ALLOCATION_MSG);
			fflush(NULL);
			//TODO free memory
			//TODO terminate the program
		}
		for (int j=0;j<3;j++) {
			arrayHist[i][j] = (int *)malloc(nBins * sizeof(int)); // [Image number][R/G/B][nBins]
			if (arrayHist[i][j] == NULL) {
				printf(ERROR_ALLOCATION_MSG);
				fflush(NULL);
				//TODO free memory
				//TODO terminate the program
			}
		}
		for (int j=0;j<maxNFeatures;j++) {
			arraySift[i][j] = (double *)malloc(128 * sizeof(double)); // [Image number][nFeatures][128]
			if (arraySift[i][j] == NULL) {
				printf(ERROR_ALLOCATION_MSG);
				fflush(NULL);
				//TODO free memory
				//TODO terminate the program
			}
		}
	}
	calcArrayHist(arrayHist,numberOfImages,nBins,dir,prefix,suffix); // spGetRGBHist(char* str, int nBins)
	calcArraySift(arraySift,numberOfImages,maxNFeatures,dir,prefix,suffix); // spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures)
	// Continue
	printf(QUERY_IMG_MSG);
	fflush(stdout);
	scanf("%1024s",queryImage);
	while (strcmp(queryImage, "#") != 0) {
		calcDistHist(closestHist,numberOfImages,nBins,queryImage,arrayHist);
		calcDistSift(closestSift,numberOfImages,maxNFeatures,queryImage,arraySift);
		printf(NEAREST_GLOBAL_MSG);
		fflush(stdout);
		printf("%d, %d, %d, %d, %d\n",closestHist[0],closestHist[1],closestHist[2],closestHist[3],closestHist[4]);
		fflush(stdout);
		printf(NEAREST_LOCAL_MSG);
		fflush(stdout);
		printf("%d, %d, %d, %d, %d\n",closestSift[0],closestSift[1],closestSift[2],closestSift[3],closestSift[4]);
		fflush(stdout);
		//TODO free temp memory
		printf(QUERY_IMG_MSG);
		fflush(stdout);
		scanf("%1024s",queryImage);
	}
	printf(EXIT_MSG);
	fflush(stdout);
	//TODO free memory
	return 0;
}


