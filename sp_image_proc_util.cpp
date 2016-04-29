#include "sp_image_proc_util.h"

int** spGetRGBHist(char* str, int nBins) {
	int **ret = (int **)malloc(3 * nBins * sizeof(int*));
	for (int j=0;j<nBins;j++) {
		ret[0][j]=j;
		ret[1][j]=2*j;
		ret[2][j]=3*j;
	}
	return ret;
}

double spRGBHistL2Distance(int** histA, int** histB, int nBins) {
	return histB[2][2];
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures) {
	double **ret = (double **)malloc(maxNFeautres * 128 * sizeof(double*));
	for (int i=0;i<maxNFeautres;i++) {
		for (int j=0;j<128;j++) {
			ret[i][j]=i+j+0.25;
			ret[i][j]=i+j+0.5;
			ret[i][j]=i+j+0.75;
		}
	}
	nFeatures = &maxNFeautres;
	return ret;
}

double spL2SquaredDistance(double* featureA, double* featureB) {
	return 1.2;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA, double*** databaseFeatures, int numberOfImages, int* nFeaturesPerImage) {
	return 0;
}
