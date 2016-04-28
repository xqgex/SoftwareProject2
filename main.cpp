#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>//Mat
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgcodecs.hpp>//imread
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor

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
	int numberOfImages,nBins,maxNFeatures;
	char dir[1025],prefix[1025],suffix[1025],queryImage[1025];
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
	//spGetRGBHist(,nBins)
	//spGetSiftDescriptors(,maxNFeautres,);
	printf(QUERY_IMG_MSG);
	fflush(stdout);
	scanf("%1024s",queryImage);
	while (strcmp(queryImage, "#") != 0) {
		printf(NEAREST_GLOBAL_MSG);
		fflush(stdout);
		printf("1, 2, 3, 4, 5\n");
		fflush(stdout);
		printf(NEAREST_LOCAL_MSG);
		fflush(stdout);
		printf("1, 2, 3, 4, 5\n");
		fflush(stdout);
		//TODO free memory
		printf(QUERY_IMG_MSG);
		fflush(stdout);
		scanf("%1024s",queryImage);
	}
	printf(EXIT_MSG);
	fflush(stdout);
	//TODO free memory
	//
	//	printf(ERROR_ALLOCATION_MSG);
	//	fflush(NULL);
	//	//TODO free memory
	//
	return 0;
}


