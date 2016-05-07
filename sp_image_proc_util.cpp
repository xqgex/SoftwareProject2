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

using namespace cv;
int** spGetRGBHist(char* str, int nBins) {
	// Function variables
	int i; // Generic loop variable
	float range[]={0,256};
	Mat img,r_hist,g_hist,b_hist;
	const float* histRange={range};
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
	img = imread(str,CV_LOAD_IMAGE_COLOR); // Load the image with colors
	if (img.empty()) { // Better be safe then sorry
		return NULL;
	}
	std::vector<Mat> bgr_planes; // Initialize calcHist parameters
	split(img, bgr_planes);
	calcHist(&bgr_planes[2],1,0,Mat(),r_hist,1,&nBins,&histRange,true,false); // Calculate red histograms
	calcHist(&bgr_planes[1],1,0,Mat(),g_hist,1,&nBins,&histRange,true,false); // Calculate green histograms
	calcHist(&bgr_planes[0],1,0,Mat(),b_hist,1,&nBins,&histRange,true,false); // Calculate blue histograms
	for (i=0;i<nBins;i++) { // Convert the histograms from Mat of floats to array of ints
		rgb_hist[0][i] = (int)r_hist.at<float>(i);
		rgb_hist[1][i] = (int)g_hist.at<float>(i);
		rgb_hist[2][i] = (int)b_hist.at<float>(i);
	}
	return rgb_hist;
}

double spRGBHistL2Distance(int** histA, int** histB, int nBins) {
	// Function variables
	int i,j; // Generic loop variables
	double binDist,colorDist; // Temp vars
	double L2Dist=0; // The result that will be returned
	// Calc distance
	for (i=0;i<3;i++) {
		colorDist = 0;
		for (j=0;j<nBins;j++) {
			binDist = (double)histA[i][j]-(double)histB[i][j]; // Calculate the difference between bins
			colorDist += binDist*binDist; // Square the difference, and sum it up
		}
		L2Dist += 0.33*colorDist; // Calculate the average of the 3 colors
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
	img = imread(str, CV_LOAD_IMAGE_GRAYSCALE); // Load the image in greyscale
	if (img.empty()) { // Better be safe then sorry
		return NULL;
	}
	std::vector<cv::KeyPoint> kp1; // Initialize detect's parameters
	Ptr<xfeatures2d::SiftDescriptorExtractor> detect = xfeatures2d::SIFT::create(maxNFeautres);

	detect->detect(img,kp1,Mat()); // Detect Sifts
	detect->compute(img,kp1,ds1); // Compute Sifts
	if (kp1.size() < maxNFeautres) {
		*nFeatures = kp1.size();
	} else {
		*nFeatures = maxNFeautres;
	}
	for (i=0;i<*nFeatures;i++) {
		sif_Desc[i] = (double *) malloc(128 * sizeof(double)); // Memory allocation for each row
		if (sif_Desc[i] == NULL) { // Memory allocation error
			return NULL;
		}
		for (j=0;j<128;j++) { // Convert the Descriptors from Mat of floats to array of doubles
			sif_Desc[i][j]=(double)ds1.at<float>(i,j);
		}
	}
	return sif_Desc;
}

double spL2SquaredDistance(double* featureA, double* featureB) {
	// Function variables
	int i; // Generic loop variable
	double L2Dist=0,entry;
	// Calc squared distance
	for (i=0;i<128;i++) {
		entry = featureA[i]-featureB[i]; //  Calculate the difference SIFTs
		L2Dist += entry*entry; // Square the result and sum it up
	}
	return L2Dist;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA, double*** databaseFeatures, int numberOfImages, int* nFeaturesPerImage) {
	// Function variables
	int i,j; // Generic loop variables
	int featCount=0;
	double featDist;
	double featThreshold;
	int *bestMatches;
	double *bestMatchesDist;
	// Allocate memory
	bestMatches = (int *)malloc(bestNFeatures * sizeof(int)); // Contain the index of the images of the best features
	bestMatchesDist = (double *)malloc(bestNFeatures * sizeof(double)); // Contain the distances of the best features
	if ((bestMatches == NULL)or(bestMatchesDist == NULL)) {
		freeMemoryDynamic(bestMatches,1, 0, 0);
		freeMemoryDynamic(bestMatchesDist,1, 0, 0);
		return NULL;
	}
	//Now calc the distance
	for (i=0;i<numberOfImages;i++) {
		for (j=0;j<nFeaturesPerImage[i];j++) {
			featDist = spL2SquaredDistance(featureA,databaseFeatures[i][j]); // Calculate the distance of any 2 features
			// Add new feature to bestMatches list
			if (featCount < bestNFeatures) { // Check if we got less then bestNfeatures so far
				featThreshold = addBestMatch(bestMatchesDist,bestMatches,featCount,featDist,i);
				featCount++;
			} else if (featDist < featThreshold) { // Check if the current image has better feature then the worse feature in the list
				featThreshold = addBestMatch(bestMatchesDist,bestMatches,bestNFeatures-1,featDist,i);
			}
		}
	}
	freeMemoryDynamic(bestMatchesDist,1, 0, 0);
	return bestMatches;
}
