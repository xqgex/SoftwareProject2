#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/**
 *
 * Return True (1) if success and False (0) if not
 */
int calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist);

/**
 *
 * Return True (1) if success and False (0) if not
 */
int calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift,int* nFeaturesPerImage);

/**
 *
 * Return
 */
double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum);

/**
 * Memory allocation for 'arrayHist' and 'arraySift'
 * Return True (1) if success and False (0) if not
 */
int arraysMemoryAllocation(int*** arrayHist, double*** arraySift, int numberOfImages, int maxNFeatures, int nBins);

/**
 * Free arrays ('arrayHist', 'arraySift', 'nFeaturesPerImage') memory allocation
 * Return void
 */
/*
void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages, int maxNFeatures);
*/
void freeMemory(void* data, int dim, int dim2length, int dim3length);

#endif /* MAIN_AUX_H_ */
