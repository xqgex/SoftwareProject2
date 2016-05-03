#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

void calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist);

void calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift,int* nFeaturesPerImage);

void freeMemory(void* data, int dim);

#endif /* MAIN_AUX_H_ */
