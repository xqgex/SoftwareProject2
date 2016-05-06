#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

void calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist);

void calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift,int* nFeaturesPerImage);

int arraysMemoryAllocation(int*** arrayHist, double*** arraySift, int numberOfImages, int maxNFeatures, int nBins);

void freeMemory(int*** arrayHist, double*** arraySift, int* nFeaturesPerImage, int numberOfImages);

double addBestMatch(double* distanceArray, int* imageArray, int insertionPoint, double distance, int imageNum);

//void freeMemory(void* data, int dim);

#endif /* MAIN_AUX_H_ */
