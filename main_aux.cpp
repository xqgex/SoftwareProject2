#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdlib.h>

int calcDistHist(int* closestHist, int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	//
	int** queryHist;
	double distance, threshold;
	double* distanceArray;
	int i; // Generic loop variable
	//
	distanceArray = (double *)malloc(5 * sizeof(double));
 	queryHist = spGetRGBHist(queryImage,nBins);
	if ((queryHist == NULL)or(distanceArray == NULL)) { // Memory allocation error
		return 0;
	}
	//
	for (i=0;i<numberOfImages;i++) {
		distance = spRGBHistL2Distance(queryHist, arrayHist[i], nBins);
		if (i<5) {
			threshold = addBestMatch(distanceArray,closestHist,i,distance,i);
		} else {
			if (distance < threshold) {
				threshold = addBestMatch(distanceArray,closestHist,4,distance,i);
			}
		}
 	}
	freeMemoryDynamic(distanceArray,1, 0, 0);
	return 1;
 }

int calcDistSift(int* closestSift, int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift, int* nFeaturesPerImage) {
	//
	int i,j; // Generic loop variables
	double** querySifts;
	int* imageHitsArray;
	int* bestMatches;
	int queryNFeatures, max;
	//
	imageHitsArray = (int *)calloc(numberOfImages,sizeof(int));
	//
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
	freeMemoryDynamic(imageHitsArray,1, 0, 0);
	return 1;
}

double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum) {
	//
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
                free(arrayHist[i][j]);
        }
        for(j=0;j<maxNFeatures;j++) {
                free(arraySift[i][j]);
        }
        free(arrayHist[i]);
        free(arraySift[i]);
    }
    free(arrayHist);
    free(arraySift);
    free(nFeaturesPerImage);
    return;
}

void freeMemoryDynamic(void* data, int dim, int dim2length, int dim3length) {
	int i;
	if (dim == 1) {// Single memory "block"
		if (data != NULL) {
			free(data);
		}
	} else {  // Free 2D or 3D array
		for (i=0;i<dim2length;i++) {
			if (((void**)data)[i] != NULL) {
				freeMemoryDynamic(((void**)data)[i],dim-1,dim3length,0); // Free sub array of D2 or single block
			} else {
				break; // If we have a NULL pointer, the rest of the array is undefined, so we have to stop
			}
		}
		free(data); // Free the array
	}
}


