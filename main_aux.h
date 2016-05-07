#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/**
 * Find 5 closest images based on hist
 * Return True (1) if success and False (0) if not
 */
int calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist);

/**
 * Find 5 closest images based on sift
 * Return True (1) if success and False (0) if not
 */
int calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift,int* nFeaturesPerImage);

/**
 *
 * Return
 */
double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum);

/**
 * Free arrays ('arrayHist', 'arraySift', 'nFeaturesPerImage') memory allocation
 * Return void
 */
void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages, int maxNFeatures);

/**
 * Free memory allocation for a given 1D\2D\3D array
 * Return void
 */
void freeMemoryDynamic(void* data, int dim, int dim2length, int dim3length);

#endif /* MAIN_AUX_H_ */
