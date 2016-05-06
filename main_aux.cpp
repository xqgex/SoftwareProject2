#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdlib.h>


void calcDistHist(int* closestHist, int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	int** queryHist;
	double distance, threshold;
	double * distanceArray;
	int i;
	closestHist = (int *)malloc(5 * sizeof(int));
	distanceArray = (double *)malloc(5 * sizeof(double));
	queryHist = spGetRGBHist(queryImage, nBins);
	for (i=0;i<numberOfImages;i++){
		distance = spRGBHistL2Distance(queryHist, arrayHist[i], nBins);
		if (i<5){
			threshold = addBestMatch(distanceArray, closestHist, i, distance, i);
		}
		else{
			if (distance < threshold){
				threshold = addBestMatch(distanceArray, closestHist, 4, distance, i);
			}
		}
	}
}

void calcDistSift(int* closestSift, int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift, int* nFeaturesPerImage) {
}
double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum){
	int tempI, i;
	double tempD;
	i=insertionPoint - 1;
	distanceArray[insertionPoint] = distance;
	imageArray[insertionPoint] = imageNum;
	while (distanceArray[i] > distance){
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


void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages) {
    int i,j;
    for(i=0;i<numberOfImages;i++)
    {
        for(j=0;j<3;j++)
        {
                free(arrayHist[i][j]);
        }
        for(j=0;j<nFeaturesPerImage[i];j++)
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
/*
void freeMemory(void* data, int dim){ // TODO test if it even works
	// free memory blocks and multidimensional arrays of memory blocks
	int length = sizeof(data)/sizeof(void*); // the size of all the pointers is always the same
	if (dim == 1) // single memory "block"
		free(data);
	else{  // array of pointers to lower dim array or single blocks
		for (int i=0;i<length;i++){
			freeMemory(((void**)data)[i],dim-1); // data is multidimensional array, free each element of the array
		}
		free(data); // free the array
	}
}
*/
