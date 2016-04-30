#include "sp_image_proc_util.h"
#define MAX_SIFT_DISTANCE 256*128
using namespace cv;

int** spGetRGBHist(char* str, int nBins) {
	int **rgb_hist = (int **)malloc(3 * sizeof(int*));
	for (int i=0;i<3;i++){
		rgb_hist[i] = (int *) malloc(nBins * sizeof(int));
	}
	Mat img;
	img = imread(str, CV_LOAD_IMAGE_COLOR); // load the image with colors
	if (img.empty()) {
		//TODO terminate the program
	}
	std::vector<Mat> bgr_planes; // initialize calcHist parameters
	split(img, bgr_planes);
	float range[] = { 0, 256 };
	const float* histRange = { range };
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &nBins, &histRange); // calculate histograms
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &nBins, &histRange);
	for (int j=0;j<nBins;j++) { // convert the histograms from Mat of floats to array of ints
		rgb_hist[0][j] = (int)r_hist.at<float>(j);
		rgb_hist[1][j] = (int)g_hist.at<float>(j);
		rgb_hist[2][j] = (int)b_hist.at<float>(j);
	}
	return rgb_hist;
}

double spRGBHistL2Distance(int** histA, int** histB, int nBins) {
	double L2Dist = 0;
	int colorDist;
	int binDist;
	for (int i=0;i<3;i++){
		colorDist = 0;
		for (int j=0;j<nBins;j++){
			binDist = histA[i][j]-histB[i][j]; // calculate the difference between bins
			colorDist += binDist*binDist; // square the differnce, and sum it up
		}
		L2Dist += 0.33 * colorDist; // calculate the average of the 3 colors
	}
	return L2Dist;
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures) {
	double **sif_Desc = (double **)malloc(maxNFeautres * sizeof(double *));
	Mat img;
	img = imread(str, CV_LOAD_IMAGE_GRAYSCALE); // load the image in greyscale
	if (img.empty()) {
		//TODO terminate the program
	}

	std::vector<cv::KeyPoint> kp1; // initialize detect's parameters
	Mat ds1;
	Ptr<xfeatures2d::SiftDescriptorExtractor> detect = xfeatures2d::SIFT::create(maxNFeautres);
	detect->detect(img, kp1, Mat()); // detect SIFTs
	detect->compute(img, kp1, ds1); // compute Sifts

	for (int i=0;i<maxNFeautres;i++) {
		sif_Desc[i] = (double *) malloc(128 * sizeof(double)); // memory allocation for each row
		for (int j=0;j<128;j++) { // convert the Descriptors from Mat of floats to array of doubles
			sif_Desc[i][j]=(double)ds1.at<float>(i,j);
		}
	}
	*nFeatures = maxNFeautres;
	return sif_Desc;
}

double spL2SquaredDistance(double* featureA, double* featureB) {
	double L2Dist = 0;
	double entry;
	for (int j=0;j<128;j++){
		entry = featureA[j]-featureB[j]; //  calculate the difference SIFTs
		L2Dist += entry*entry; // square the result and sum it up
	}
	return L2Dist;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA, double*** databaseFeatures, int numberOfImages, int* nFeaturesPerImage) {
	int *bestMatches = (int *)malloc(bestNFeatures * 128 * sizeof(int)); // contain the index of the images of the best features
	int *bestMatchesDist = (int *)malloc(bestNFeatures * 128 * sizeof(int)); // contain the distances of the best features
	double featDist;
	double minimalDist;
	double featThreshold = 0;
	int featThresholdIndex;
	int resultCount = 0;
	int temp; // temp varible for the sort
	for (int i=0;i<numberOfImages;i++){
		minimalDist = MAX_SIFT_DISTANCE;
		for (int j=0;j<nFeaturesPerImage[i];j++){
			featDist = spL2SquaredDistance(featureA,databaseFeatures[i][j]); // calculate the distance of any 2 features
			if (featDist < minimalDist){
				minimalDist = featDist; // save the minimal distance of each image
			}
		}
		// add new image to bestMatches list
		if (resultCount < bestNFeatures){ // check if we got less then bestNfeatures so far
			bestMatches[resultCount] = i;
			bestMatchesDist[resultCount] = minimalDist;
			resultCount++;
			if (minimalDist > featThreshold){ // save the highest distance and his index in the bestMatches list
				featThreshold = minimalDist;
				featThresholdIndex = resultCount - 1;
			}
		}
		else{
			if (minimalDist < featThreshold){ // check if the current image has better feature then the worse feature in the list
				bestMatches[featThresholdIndex] = i;
				bestMatchesDist[featThresholdIndex] = minimalDist;
				featThreshold = 0;
				for (int k=0;k<bestNFeatures;k++){ // find the new threshold (highest distance in the bestMatch list)
					if(featThreshold < bestMatchesDist[k]){
						featThresholdIndex = k;
						featThreshold = bestMatchesDist[k];
					}
				}
			}
		}
		// sort the results in ascending order
		for (int i=0;i<resultCount;i++){
			featThreshold = MAX_SIFT_DISTANCE;
			for (int j=i;j<resultCount;j++){ // find the minimal distance from the remaining images
				if (bestMatchesDist[j] < featThreshold){
					featThreshold = bestMatchesDist[j];
					featThresholdIndex = j;
				}
			}
			temp = bestMatches[i]; // move the minimal distance image to the right place
			bestMatches[i] = bestMatches[featThresholdIndex];
			bestMatches[featThresholdIndex] = temp;
			temp = bestMatchesDist[i];
			bestMatchesDist[i] = bestMatchesDist[featThresholdIndex];
			bestMatchesDist[featThresholdIndex] = temp;
		}
	}
	return bestMatches;
}
