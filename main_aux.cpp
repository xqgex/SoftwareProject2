#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdlib.h>

int calcDistHist(int* closestHist, int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	//
	int** queryHist;
	double distance, threshold;
	double* distanceArray;
	int i;
	//
	distanceArray = (double *)malloc(5 * sizeof(double));
 	queryHist = spGetRGBHist(queryImage, nBins);
	if ((queryHist == NULL)or(distanceArray == NULL)) { // Memory allocation error
		return 0;
	}
	//
	for (i=0;i<numberOfImages;i++) {
		distance = spRGBHistL2Distance(queryHist, arrayHist[i], nBins);
		if (i<5) {
			threshold = addBestMatch(distanceArray, closestHist, i, distance, i);
		} else {
			if (distance < threshold) {
				threshold = addBestMatch(distanceArray, closestHist, 4, distance, i);
			}
		}
 	}
	freeMemory(distanceArray,1, 0, 0);
	return 1;
 }

int calcDistSift(int* closestSift, int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift, int* nFeaturesPerImage) {
	double** querySifts;
	int* imageHitsArray;
	int* bestMatches;
	int queryNFeatures, i,j, max;
	imageHitsArray = (int *)malloc(numberOfImages * sizeof(int));
	for (i=0;i<numberOfImages;i++){
		imageHitsArray[i]=0;
	}
	querySifts = spGetSiftDescriptors(queryImage, maxNFeatures, &queryNFeatures);
	for (i=0;i<queryNFeatures;i++){
		bestMatches = spBestSIFTL2SquaredDistance(5, querySifts[i],arraySift,numberOfImages,nFeaturesPerImage);
		for (j=0;j<5;j++){
			imageHitsArray[bestMatches[j]]++;
		}
	}
	for (i=0;i<5;i++){
		max = 0;
		for (j=0;j<numberOfImages;j++){
			if (imageHitsArray[j] > max){
				max = imageHitsArray[j];
				closestSift[i] = j;
			}
		}
		imageHitsArray[closestSift[i]]=0;
	}
	freeMemory(imageHitsArray,1, 0, 0);
	return 1;
}

double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum) {
	//
	int tempI, i;
	double tempD;
	//
	i=insertionPoint - 1;
	distanceArray[insertionPoint] = distance;
	imageArray[insertionPoint] = imageNum;
	while (distanceArray[i] > distance) {
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
/*
int arraysMemoryAllocation(int*** arrayHist, double*** arraySift, int numberOfImages, int maxNFeatures, int nBins) {
	int i,j;
	for (i=0;i<numberOfImages;i++) {
		arrayHist[i] = (int **)malloc(3 * sizeof(int*)); // [Image number][R/G/B][nBins]
		arraySift[i] = (double **)malloc(maxNFeatures * sizeof(double*)); // [Image number][nFeatures][128]
		if ((arrayHist[i] == NULL)or(arraySift[i] == NULL)) {
			return 0;
		}
		for (j=0;j<3;j++) {
			arrayHist[i][j] = (int *)malloc(nBins * sizeof(int)); // [Image number][R/G/B][nBins]
			if (arrayHist[i][j] == NULL) {
				return 0;
			}
		}
		for (j=0;j<maxNFeatures;j++) {
			arraySift[i][j] = (double *)malloc(128 * sizeof(double)); // [Image number][nFeatures][128]
			if (arraySift[i][j] == NULL) {
				return 0;
			}
		}
	}
	return 1;
}
*/
/*
void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages, int maxNFeatures) {
    int i,j;
    for(i=0;i<numberOfImages;i++)
    {
        for(j=0;j<3;j++)
        {
                free(arrayHist[i][j]);
        }
        for(j=0;j<maxNFeatures;j++)
        {
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
*/
void freeMemory(void* data, int dim, int dim2length, int dim3length){
	// free memory blocks and 2D\3D arrays of memory blocks
	int i;
	if (dim == 1) // single memory "block"
		if (data != NULL)
			free(data);
	else{  // free 2D or 3D array
		for (i=0;i<dim2length;i++){
			if (((void**)data)[i] != NULL)
				freeMemory(((void**)data)[i],dim-1,dim3length,0); // free sub array of D2 or single block
			else
				break; // if we have a NULL pointer, the rest of the array is undefined, so we have to stop
		}
		free(data); // free the array
	}
}


