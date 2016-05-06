#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	// Input variables
	int numberOfImages,nBins,maxNFeatures;
	char dir[1025],prefix[1025],suffix[1025],queryImage[1025],path[4097];
	// Program variables
	int i; // Generic loop variable
	int closestHist[5],closestSift[5]; // Five closest images
	int mallocDistHistSuccess,mallocDistSiftSuccess; // Store True (1) if there wasn't memory allocation problem
	int ***arrayHist; // arrayHist = [Image number][R/G/B][nBins]
	double ***arraySift; // arraySift = [Image number][nFeatures][128]
	int *nFeaturesPerImage;
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
		return (EXIT_FAILURE);
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
		return (EXIT_FAILURE);
	}
	printf(NUM_FEATURES_MSG);
	fflush(stdout);
	scanf("%d",&maxNFeatures);
	if (maxNFeatures<1) {
		printf(ERROR_NUM_FEATURES_MSG);
		fflush(stdout);
		return (EXIT_FAILURE);
	}
	// Allocate memory
	arrayHist = (int ***)malloc(numberOfImages * sizeof(int**));
	arraySift = (double ***)malloc(numberOfImages * sizeof(double**));
	nFeaturesPerImage = (int *)malloc(numberOfImages * sizeof(int));
	if ( (arrayHist == NULL)or(arraySift == NULL)or(nFeaturesPerImage == NULL) ) { // Memory allocation error
		printf(ERROR_ALLOCATION_MSG);
		fflush(NULL);
		freeMemory(arrayHist,3,numberOfImages,3);
		freeMemory(arraySift,3,numberOfImages,maxNFeatures);
		freeMemory(nFeaturesPerImage,1, 0, 0);
		return (EXIT_FAILURE);
	}
	// Preproccessing
	for (i=0;i<numberOfImages;i++) {
		snprintf(path,sizeof path,"%s%s%d%s",dir,prefix,i,suffix);
		arrayHist[i] = spGetRGBHist(path,nBins);
		arraySift[i] = spGetSiftDescriptors(path,maxNFeatures,&(nFeaturesPerImage[i]));
		if ((arrayHist[i] == NULL)or(arraySift[i] == NULL)) { // Memory allocation error
			printf(ERROR_ALLOCATION_MSG);
			fflush(NULL);
			freeMemory(arrayHist,3,numberOfImages,3);
			freeMemory(arraySift,3,numberOfImages,maxNFeatures);
			return (EXIT_FAILURE);
		}
	}
	// Start input loop
	printf(QUERY_IMG_MSG);
	fflush(stdout);
	scanf("%1024s",queryImage);
	while (strcmp(queryImage,"#") != 0) {
		mallocDistHistSuccess = calcDistHist(closestHist,numberOfImages,nBins,queryImage,arrayHist);
		mallocDistSiftSuccess = calcDistSift(closestSift,numberOfImages,maxNFeatures,queryImage,arraySift,nFeaturesPerImage);
		if ((mallocDistHistSuccess == 0)or(mallocDistSiftSuccess == 0)) { // Memory allocation error
			printf(ERROR_ALLOCATION_MSG);
			fflush(NULL);
			freeMemory(arrayHist,3,numberOfImages,3);
			freeMemory(arraySift,3,numberOfImages,maxNFeatures);
			freeMemory(nFeaturesPerImage,1,0,0);
			return (EXIT_FAILURE);
		}
		printf(NEAREST_GLOBAL_MSG);
		fflush(stdout);
		printf("%d, %d, %d, %d, %d\n",closestHist[0],closestHist[1],closestHist[2],closestHist[3],closestHist[4]);
		fflush(stdout);
		printf(NEAREST_LOCAL_MSG);
		fflush(stdout);
		printf("%d, %d, %d, %d, %d\n",closestSift[0],closestSift[1],closestSift[2],closestSift[3],closestSift[4]);
		fflush(stdout);
		//TODO free temp memory
		//XXX Do we have such memory?
		printf(QUERY_IMG_MSG);
		fflush(stdout);
		scanf("%1024s",queryImage);
	}
	printf(EXIT_MSG);
	fflush(stdout);
	freeMemory(arrayHist,3,numberOfImages,3);
	freeMemory(arraySift,3,numberOfImages,maxNFeatures);
	freeMemory(nFeaturesPerImage,1,0,0);
	return (EXIT_SUCCESS);
}
