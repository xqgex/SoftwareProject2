#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

void calcArrayHist(int*** arrayHist,int numberOfImages, int nBins, char* dir, char* prefix, char* suffix);

void calcArraySift(double*** arraySift,int numberOfImages, int maxNFeatures, char* dir, char* prefix, char* suffix);

void calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist);

void calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift);

#endif /* MAIN_AUX_H_ */
