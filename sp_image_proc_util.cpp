#include "sp_image_proc_util.h"
#include "main_aux.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>//Mat
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgcodecs.hpp>//imread
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor

#define MAX_SIFT_DISTANCE 256*128
using namespace cv;

int** spGetRGBHist(char* str, int nBins) {
	// Function variables
	int i; // Generic loop variable
	float range[]={0,256};
	const float* histRange={range};
	Mat img,b_hist,g_hist,r_hist;
	int **rgb_hist;
	// Allocate memory
	rgb_hist = (int **)malloc(3 * sizeof(int*));
	if (rgb_hist == NULL) { // Memory allocation error
		return NULL;
	}
	for (i=0;i<3;i++) {
		rgb_hist[i] = (int *) malloc(nBins * sizeof(int));
		if (rgb_hist[i] == NULL) { // Memory allocation error
			return NULL;
		}
	}
	// Calc RGB hist
	img = imread(str, CV_LOAD_IMAGE_COLOR); // load the image with colors
	if (img.empty()) { // Better be safe then sorry
		return NULL;
	}
	std::vector<Mat> bgr_planes; // initialize calcHist parameters
	split(img, bgr_planes);
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &nBins, &histRange); // calculate histograms
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &nBins, &histRange);
	for (i=0;i<nBins;i++) { // convert the histograms from Mat of floats to array of ints
		rgb_hist[0][i] = (int)r_hist.at<float>(i);
		rgb_hist[1][i] = (int)g_hist.at<float>(i);
		rgb_hist[2][i] = (int)b_hist.at<float>(i);
	}
	return rgb_hist;
}

double spRGBHistL2Distance(int** histA, int** histB, int nBins) {
	// Function variables
	double L2Dist=0;
	int colorDist,binDist;
	int i,j; // Generic loop variables
	// Calc distance
	for (i=0;i<3;i++) {
		colorDist = 0;
		for (j=0;j<nBins;j++) {
			binDist = histA[i][j]-histB[i][j]; // calculate the difference between bins
			colorDist += binDist*binDist; // square the differnce, and sum it up
		}
		L2Dist += 0.33*colorDist; // calculate the average of the 3 colors
	}
	return L2Dist;
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures) {
	// Function variables
	int i,j; // Generic loop variables
	Mat img,ds1;
	double **sif_Desc;
	// Allocate memory
	sif_Desc = (double **)malloc(maxNFeautres * sizeof(double *));
	if (sif_Desc == NULL) { // Memory allocation error
		return NULL;
	}
	// Calc sift descriptors
	img = imread(str, CV_LOAD_IMAGE_GRAYSCALE); // load the image in greyscale
	if (img.empty()) { // Better be safe then sorry
		return NULL;
	}
	std::vector<cv::KeyPoint> kp1; // initialize detect's parameters
	Ptr<xfeatures2d::SiftDescriptorExtractor> detect = xfeatures2d::SIFT::create(maxNFeautres);
	detect->detect(img, kp1, Mat()); // detect Sifts
	detect->compute(img, kp1, ds1); // compute Sifts
	for (i=0;i<maxNFeautres;i++) {
		sif_Desc[i] = (double *) malloc(128 * sizeof(double)); // memory allocation for each row
		if (sif_Desc[i] == NULL) { // Memory allocation error
			return NULL;
		}
		for (j=0;j<128;j++) { // convert the Descriptors from Mat of floats to array of doubles
			sif_Desc[i][j]=(double)ds1.at<float>(i,j);
		}
	}
	*nFeatures = maxNFeautres;
	return sif_Desc;
}

double spL2SquaredDistance(double* featureA, double* featureB) {
	// Function variables
	int i; // Generic loop variable
	double L2Dist=0,entry;
	// Calc squared distance
	for (i=0;i<128;i++){
		entry = featureA[i]-featureB[i]; //  calculate the difference SIFTs
		L2Dist += entry*entry; // square the result and sum it up
	}
	return L2Dist;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA, double*** databaseFeatures, int numberOfImages, int* nFeaturesPerImage) {
	// Function variables
	int i,j,k; // Generic loop variables
	int *bestMatches,*bestMatchesDist;
	double featDist;
	double minimalDist;
	double featThreshold=0;
	int featThresholdIndex;
	int resultCount=0;
	int sortHelpVar; // Temporary variable for the sort
	// Allocate memory
	bestMatches = (int *)malloc(bestNFeatures * 128 * sizeof(int)); // Contain the index of the images of the best features
	bestMatchesDist = (int *)malloc(bestNFeatures * 128 * sizeof(int)); // Contain the distances of the best features
	if ((bestMatches == NULL)or(bestMatchesDist == NULL)) { // Memory allocation error
		return NULL;
	}
	//
	for (i=0;i<numberOfImages;i++){
		minimalDist = MAX_SIFT_DISTANCE;
		for (j=0;j<nFeaturesPerImage[i];j++) {
			featDist = spL2SquaredDistance(featureA,databaseFeatures[i][j]); // Calculate the distance of any 2 features
			if (featDist < minimalDist) {
				minimalDist = featDist; // Save the minimal distance of each image
			}
		}
		// Add new image to bestMatches list
		if (resultCount < bestNFeatures) { // Check if we got less then bestNfeatures so far
			bestMatches[resultCount] = i;
			bestMatchesDist[resultCount] = minimalDist;
			resultCount++;
			if (minimalDist > featThreshold) { // Save the highest distance and his index in the bestMatches list
				featThreshold = minimalDist;
				featThresholdIndex = resultCount - 1;
			}
		} else {
			if (minimalDist < featThreshold) { // Check if the current image has better feature then the worse feature in the list
				bestMatches[featThresholdIndex] = i;
				bestMatchesDist[featThresholdIndex] = minimalDist;
				featThreshold = 0;
				for (j=0;j<bestNFeatures;j++) { // Find the new threshold (highest distance in the bestMatch list)
					if(featThreshold < bestMatchesDist[j]) {
						featThresholdIndex = j;
						featThreshold = bestMatchesDist[j];
					}
				}
			}
		}
		// Sort the results in ascending order
		for (j=0;j<resultCount;j++){
			featThreshold = MAX_SIFT_DISTANCE;
			for (k=j;k<resultCount;k++) { // Find the minimal distance from the remaining images
				if (bestMatchesDist[k] < featThreshold) {
					featThreshold = bestMatchesDist[k];
					featThresholdIndex = k;
				}
			}
			sortHelpVar = bestMatches[j]; // Move the minimal distance image to the right place
			bestMatches[j] = bestMatches[featThresholdIndex];
			bestMatches[featThresholdIndex] = sortHelpVar;
			sortHelpVar = bestMatchesDist[j];
			bestMatchesDist[j] = bestMatchesDist[featThresholdIndex];
			bestMatchesDist[featThresholdIndex] = sortHelpVar;
			// TODO Consider using of qsort instead
		}
	}
	return bestMatches;
}
