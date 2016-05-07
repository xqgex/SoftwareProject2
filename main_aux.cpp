#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdlib.h> // malloc, free, NULL

int calcDistHist(int* closestHist, int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	// Function variables
	int i; // Generic loop variable
	double distance, threshold;
	double* distanceArray;
	int** queryHist;
	// Allocate memory
	distanceArray = (double *)malloc(5 * sizeof(double));
 	queryHist = spGetRGBHist(queryImage,nBins);
	if ((queryHist == NULL)or(distanceArray == NULL)) { // Memory allocation error
		return 0;
	}
	// Save 5 closest hist into 'closestHist'
	for (i=0;i<numberOfImages;i++) {
		distance = spRGBHistL2Distance(queryHist,arrayHist[i],nBins);
		if (i<5) {
			threshold = addBestMatch(distanceArray,closestHist,i,distance,i);
		} else {
			if (distance < threshold) {
				threshold = addBestMatch(distanceArray,closestHist,4,distance,i);
			}
		}
 	}
	// Free memory and return True (1)
	freeMemoryDynamic(distanceArray,1,0,0);
	freeMemoryDynamic(queryHist,2,3,0);
	return 1;
 }

int calcDistSift(int* closestSift, int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift, int* nFeaturesPerImage) {
	// Function variables
	int i,j; // Generic loop variables
	int queryNFeatures, max;
	int* imageHitsArray;
	int* bestMatches;
	double** querySifts;
	// Allocate memory
	imageHitsArray = (int *)calloc(numberOfImages,sizeof(int));
	if (imageHitsArray == NULL) { // Memory allocation error
		return 0;
	}
	// Save 5 closest sift into 'closestSift'
	querySifts = spGetSiftDescriptors(queryImage,maxNFeatures,&queryNFeatures);
	for (i=0;i<queryNFeatures;i++) {
		bestMatches = spBestSIFTL2SquaredDistance(5,querySifts[i],arraySift,numberOfImages,nFeaturesPerImage);
		for (j=0;j<5;j++) {
			imageHitsArray[bestMatches[j]]++;
		}
	}
	for (i=0;i<5;i++) {
		max = 0;
		for (j=0;j<numberOfImages;j++) {
			if (imageHitsArray[j] > max) {
				max = imageHitsArray[j];
				closestSift[i] = j;
			}
		}
		imageHitsArray[closestSift[i]] = 0;
	}
	// Free memory and return True (1)
	freeMemoryDynamic(imageHitsArray,1, 0, 0);
	freeMemoryDynamic(querySifts,2, maxNFeatures, 0);
	return 1;
}

double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum) {
	// Function variables
	int tempI, i;
	double tempD;
	//
	i = insertionPoint - 1;
	distanceArray[insertionPoint] = distance;
	imageArray[insertionPoint] = imageNum;
	while ((i>=0) and (distanceArray[i] > distance)) {
		tempD = distanceArray[i+1];
		distanceArray[i+1] = distanceArray[i];
		distanceArray[i] = tempD;
		tempI = imageArray[i+1];
		imageArray[i+1] = imageArray[i];
		imageArray[i] = tempI;
		i--;
 	}
	return distanceArray[insertionPoint]; // the new threshold
}

void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages, int maxNFeatures) {
    int i,j;
    for(i=0;i<numberOfImages;i++) {
        for(j=0;j<3;j++) {
        	if (arrayHist[i][j]) { // A tiny chance for errors in some compilers
                free(arrayHist[i][j]);
        	}
        }
        for(j=0;j<maxNFeatures;j++) {
        	if (arrayHist[i][j]) { // A tiny chance for errors in some compilers
                free(arraySift[i][j]);
        	}
        }
        if (arrayHist[i]) { // A tiny chance for errors in some compilers
        	free(arrayHist[i]);
        }
        if (arraySift[i]) { // A tiny chance for errors in some compilers
        	free(arraySift[i]);
        }
    }
    if (arrayHist) { // A tiny chance for errors in some compilers
    	free(arrayHist);
    	arrayHist = NULL; // Preventing a "double-free"
    }
    if (arraySift) { // A tiny chance for errors in some compilers
    	free(arraySift);
    	arraySift = NULL; // Preventing a "double-free"
    }
    if (nFeaturesPerImage) { // A tiny chance for errors in some compilers
    	free(nFeaturesPerImage);
    	nFeaturesPerImage = NULL; // Preventing a "double-free"
    }
    return;
}

void freeMemoryDynamic(void* data, int dim, int dim2length, int dim3length) {
	int i;
	if (dim == 1) { // Single memory "block"
		if (data) { // A tiny chance for errors in some compilers
			free(data);
			data = NULL; // Preventing a "double-free"
		}
	} else { // Free 2D or 3D array
		for (i=0;i<dim2length;i++) {
			if (((void**)data)[i] != NULL) {
				freeMemoryDynamic(((void**)data)[i],dim-1,dim3length,0); // Free sub array of D2 or single block
			} else {
				break; // If we have a NULL pointer, the rest of the array is undefined, so we have to stop
			}
		}
		if (data) { // A tiny chance for errors in some compilers
			free(data); // Free the array
			data = NULL; // Preventing a "double-free"
		}
	}
}
