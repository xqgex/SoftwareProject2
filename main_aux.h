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
 * distanceArray of imageArray are pair of arrays of distances and images respectively sorted by distances.
 * distance and imageNum are pair of new elements to insert.
 * Add the new elements to the arrays in place of the elements in the index 'insertionPoint'
 * Keep the array's entries paired and the distances array sorted.
 * Return the new threshold for the arrays (highest distance element).
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
