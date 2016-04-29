#include "main_aux.h"

void calcArrayHist(int*** arrayHist,int numberOfImages, int nBins, char* dir, char* prefix, char* suffix) {
	return;
}

void calcArraySift(double*** arraySift,int numberOfImages, int maxNFeatures, char* dir, char* prefix, char* suffix) {
	return;
}

void calcDistHist(int* retArray,int numberOfImages, int nBins, char* queryImage, int*** arrayHist) {
	for (int i=0;i<5;i++) {
		retArray[i]=i;
	}
	return;
}

void calcDistSift(int* retArray,int numberOfImages, int maxNFeatures, char* queryImage, double*** arraySift) {
	for (int i=0;i<5;i++) {
		retArray[i]=5-i;
	}
	return;
}
