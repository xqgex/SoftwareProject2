#include "main_aux.h"
#include "sp_image_proc_util.h"
#include <stdlib.h>

int calcDistHist(int* retArray, int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	int** queryHist; //FIXME "variable 'queryHist' set but not used"
	queryHist = spGetRGBHist(queryImage, nBins);
	if (queryHist == NULL) { // Memory allocation error
		return 0;
	}
	for (int i=0;i<numberOfImages;i++){
		//spRGBHistL2Distance(...)
		//TODO finish this
	}
	return 1;
}

int calcDistSift(int* retArray, int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift, int* nFeaturesPerImage) {
	//spBestSIFTL2SquaredDistance(...)
	//TODO complicated
	return 1;
}

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
