#include "main_aux.h"
#include "sp_image_proc_util.h"

void calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	int** queryHist;

	queryHist = spGetRGBHist(queryImage, nBins);
	for (int i=0;i<numberOfImages;i++){
		//TODO finish this
	}
	return;
}

void calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift,int* nFeaturesPerImage) {

	//TODO complicated

	return;
}

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
