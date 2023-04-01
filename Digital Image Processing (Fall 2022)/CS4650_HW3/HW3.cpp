#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core.hpp"
#include <iostream>
#include <string>
#include <climits>

using namespace cv;
using namespace std;

Mat kMeansColor(Mat img, int numOfClusters);
Mat kMeansIntensity(Mat img, int numOfClusters);

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "Invalid number of arguments! " << argc << "\n";
		exit(0);
	}

	Mat malariaColor = imread(argv[1], IMREAD_COLOR);
	Mat malariaGray = imread(argv[1], IMREAD_GRAYSCALE);

	imwrite("malariaGray.png", malariaGray);

	Mat malariaLaplacian = malariaGray.clone();
	Mat malariaFiltered = malariaColor.clone();

	// This value dictates the number of clusters to be ran in each k-means method call //
	int numberOfKmeansClusters = 4;

	cout << "\n Running k-means operations with k = " << numberOfKmeansClusters << "\n";

	imshow("Malaria Input Image", malariaColor);
	waitKey(0);
	
	Mat malariaKmeansColor = kMeansColor(malariaColor, numberOfKmeansClusters);
	imshow("Malaria Image after k-means with Feature = Color", malariaKmeansColor);
	imwrite("malariaKmeansColor.png", malariaKmeansColor);
	waitKey(0);

	Mat malariaKmeansMask;
	Mat dilateMat = Mat::ones(2, 2, 0);
	Mat erodeMat = Mat::ones(2, 2, 0);
	cvtColor(malariaKmeansColor, malariaKmeansMask, COLOR_BGR2GRAY);

	// Thresholding operation
	for (int i = 0; i < malariaKmeansMask.rows; i++) {
		for (int j = 0; j < malariaKmeansMask.cols; j++) {
			if (malariaKmeansMask.at<uchar>(i, j) < 90) {
				malariaKmeansMask.at<uchar>(i, j) = 255;
			}
			else {
				malariaKmeansMask.at<uchar>(i, j) = 0;
			}
		}
	}

	dilate(malariaKmeansMask, malariaKmeansMask, dilateMat);
	erode(malariaKmeansMask, malariaKmeansMask, erodeMat);

	imshow("Malaria k-means (Binarized) Before Hole Filling", malariaKmeansMask);

	Mat malariaFilled = malariaKmeansMask.clone();
	floodFill(malariaFilled, Point(0, 0), Scalar(255));

	Mat malariaFilledInverse;
	bitwise_not(malariaFilled, malariaFilledInverse);

	malariaKmeansMask = (malariaKmeansMask | malariaFilledInverse);

	imshow("Malaria k-means (Binarized) After Hole Filling", malariaKmeansMask);
	imwrite("malariaKmeansBinarized.png", malariaKmeansMask);
	waitKey(0);

	Mat distTransform;
	distanceTransform(malariaKmeansMask, distTransform, DIST_L2, 3);
	normalize(distTransform, distTransform, 0, 1.0, NORM_MINMAX);
	
	imshow("Malaria Distance Transform", distTransform);
	imwrite("malariaDistanceTransform.png", distTransform);
	waitKey(0);

	threshold(distTransform, distTransform, 0.65, 1.0, THRESH_BINARY);

	imshow("Malaria Distance Transform (Binarized)", distTransform);
	imwrite("malariaDistanceTransformBinarized.png", distTransform);
	waitKey(0);

	dilate(distTransform, distTransform, dilateMat);

	Mat distTransformColor;
	Mat markers = Mat::zeros(distTransform.size(), CV_32S);
	distTransform.convertTo(distTransformColor, 0);
	vector<vector<Point>> edges;

	findContours(distTransformColor, edges, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < edges.size(); i++)
	{
		drawContours(markers, edges, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
	}
	
	circle(markers, Point(5, 5), 3, Scalar(255), -1);
	Mat markerVisual;
	
	watershed(malariaKmeansColor, markers);

	markers.convertTo(markerVisual, 0, 10);
	imshow("Malaria Markers", markerVisual);
	imwrite("malariaMarkers.png", markerVisual);
	waitKey(0);

	Mat markersInverted;
	markers.convertTo(markersInverted, 0);
	bitwise_not(markersInverted, markersInverted);

	Mat labels(markersInverted.size(), CV_32S);
	int cc = connectedComponents(markersInverted, labels);
	cout << "\n" << cc << "\n";
	int* regionAreas = new int[cc];
	for (int i = 0; i < cc; i++) {
		regionAreas[i] = 0;
	}

	int** colors = new int* [cc];
	for (int i = 0; i < cc; i++) {
		colors[i] = new int[4];
		colors[i][0] = 0;
		colors[i][1] = 0;
		colors[i][2] = 0;
		colors[i][3] = 0;
	}
	
	for (int i = 0; i < malariaKmeansColor.rows; i++) {
		for (int j = 0; j < malariaKmeansColor.cols; j++) {
			int pixelLabel = labels.at<int>(i, j);
			regionAreas[pixelLabel] += 1;

			colors[pixelLabel][0] += malariaKmeansColor.at<Vec3b>(i, j)[0];
			colors[pixelLabel][1] += malariaKmeansColor.at<Vec3b>(i, j)[1];
			colors[pixelLabel][2] += malariaKmeansColor.at<Vec3b>(i, j)[2];
			colors[pixelLabel][3] += 1;
		}
	}
	
	for (int i = 0; i < cc; i++) {
		colors[i][0] = colors[i][0] / colors[i][3];
		colors[i][1] = colors[i][1] / colors[i][3];
		colors[i][2] = colors[i][2] / colors[i][3];

		if (i == 0) {
			cout << "Background Area: " << regionAreas[i] << "\n";
		}
		else {
			cout << "Cell " << i << " Area: " << regionAreas[i] << " pixels.\n";
			cout << "Cell " << i << " Mean Color(BGR): " << colors[i][0] << " " << colors[i][1] << " " << colors[i][2] << "\n";
		}
	}

	Mat malariaMeanColors = malariaKmeansColor.clone();
	for (int i = 0; i < malariaKmeansColor.rows; i++) {
		for (int j = 0; j < malariaKmeansColor.cols; j++) {
			int pixelLabel = labels.at<int>(i, j);
			Vec3b color;
			color[0] = colors[pixelLabel][0];
			color[1] = colors[pixelLabel][1];
			color[2] = colors[pixelLabel][2];

			malariaMeanColors.at<Vec3b>(i,j) = color;
		}
	}

	imshow("Malaria Image Mean Colors", malariaMeanColors);
	imwrite("malariaMeanColors.png", malariaMeanColors);
	waitKey(0);

	return 0;
}

Mat kMeansColor(Mat img, int numOfClusters) {
	Mat kMeansImg = img.clone();
	int* pixelClusters = new int[img.rows * img.cols](); // Holds the cluster each pixel is assigned to.
	bool done = false;

	// Create array that holds cluster centers
	int** centers = new int* [numOfClusters];
	for (int i = 0; i < numOfClusters; i++) {
		centers[i] = new int[3];

		// Initialize each center to be a random color
		centers[i][0] = rand() % 255;
		centers[i][1] = rand() % 255;
		centers[i][2] = rand() % 255;
	}
	
	// Loop that ends when no cluster centers are changed
	while (!done) {

		// 2D array that will hold the number of pixels and running sum of pixel colors for each cluster
		int** clusterValues = new int* [numOfClusters];
		for (int i = 0; i < numOfClusters; i++) {
			clusterValues[i] = new int[4];
			clusterValues[i][0] = 0; // Color channel 1
			clusterValues[i][1] = 0; // Color channel 2
			clusterValues[i][2] = 0; // Color channel 3
			clusterValues[i][3] = 0; // Number of pixels in cluster
		}

		// Run through every pixel and assign cluster based on color
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {


				// Extract Color from current pixel. //
				Vec3b color = img.at<Vec3b>(i, j);


				int assignedCluster = 0;

				for (int k = 0; k < numOfClusters; k++) {

					int newClusterDistance = abs(color[0] - centers[k][0]) + abs(color[1] - centers[k][1]) + abs(color[2] - centers[k][2]);

					int currentClusterDistance = abs(color[0] - centers[assignedCluster][0]) + abs(color[1] - centers[assignedCluster][1]) + abs(color[2] - centers[assignedCluster][2]);

					if (newClusterDistance < currentClusterDistance) { // If pixel mean is closer to a cluster that isn't the one it's currently assigned to
						assignedCluster = k; // Then assign pixel to new cluster
					}
				}

				pixelClusters[i * img.cols + j] = assignedCluster;
				clusterValues[assignedCluster][0] += color[0];
				clusterValues[assignedCluster][1] += color[1];
				clusterValues[assignedCluster][2] += color[2];
				clusterValues[assignedCluster][3] += 1;
			}
		}

		// Check for cluster centers that have changed
		bool unchanged = true;
		for (int i = 0; i < numOfClusters; i++) {

			if (clusterValues[i][3] > 0) { // Checks for edge Case check for cluster with 0 pixels

				if (clusterValues[i][0] / clusterValues[i][3] != centers[i][0]) {
					centers[i][0] = clusterValues[i][0] / clusterValues[i][3];
					unchanged = false;
				}

				if (clusterValues[i][1] / clusterValues[i][3] != centers[i][1]) {
					centers[i][1] = clusterValues[i][1] / clusterValues[i][3];
					unchanged = false;
				}

				if (clusterValues[i][2] / clusterValues[i][3] != centers[i][2]) {
					centers[i][2] = clusterValues[i][2] / clusterValues[i][3];
					unchanged = false;
				}
			}
		}

		if (unchanged) { // Check for no change in centers, indicating an end to the encapsulating while loop
			done = true;
		}

		for (int i = 0; i < numOfClusters; i++) {
			delete[] clusterValues[i];
		}
		delete[] clusterValues;
	}

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			kMeansImg.at<Vec3b>(i, j)[0] = centers[pixelClusters[i * img.cols + j]][0];
			kMeansImg.at<Vec3b>(i, j)[1] = centers[pixelClusters[i * img.cols + j]][1];
			kMeansImg.at<Vec3b>(i, j)[2] = centers[pixelClusters[i * img.cols + j]][2];
		}
	}

	// Free array memory
	for (int i = 0; i < numOfClusters; i++) {
		delete[] centers[i];
	}
	delete[] centers;
	delete[] pixelClusters;

	return kMeansImg;
}


Mat kMeansIntensity(Mat img, int numOfClusters) {
	Mat kMeansImg = img.clone();
	int* pixelClusters = new int[img.rows * img.cols](); // Holds the cluster each pixel is assigned to.
	bool done = false;

	// Create array that holds cluster centers
	int* centers = new int[numOfClusters];
	for (int i = 0; i < numOfClusters; i++) {
		centers[i] = rand() % 255;
	}

	// Loop that ends when no cluster centers are changed
	while (!done) {

		// 2D array that will hold the number of pixels and running sum of pixel colors for each cluster
		int** clusterValues = new int* [numOfClusters];
		for (int i = 0; i < numOfClusters; i++) {
			clusterValues[i] = new int[2];
			clusterValues[i][0] = 0; // Intensity channel
			clusterValues[i][1] = 0; // Number of pixels in cluster
		}

		// Run through every pixel and assign cluster based on color
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {


				// Extract Color from current pixel. //
				uchar color = img.at<uchar>(i, j);


				int assignedCluster = 0;

				for (int k = 0; k < numOfClusters; k++) {

					int newClusterDistance = abs(color - centers[k]);

					int currentClusterDistance = abs(color - centers[assignedCluster]);

					if (newClusterDistance < currentClusterDistance) { // If pixel mean is closer to a cluster that isn't the one it's currently assigned to
						assignedCluster = k; // Then assign pixel to new cluster
					}
				}

				pixelClusters[i * img.cols + j] = assignedCluster;
				clusterValues[assignedCluster][0] += color;
				clusterValues[assignedCluster][1] += 1;
			}
		}

		// Check for cluster centers that have changed
		bool unchanged = true;
		for (int i = 0; i < numOfClusters; i++) {

			if (clusterValues[i][3] > 0) { // Edge Case check for cluster with 0 pixels

				if (clusterValues[i][0] / clusterValues[i][1] != centers[i]) {
					centers[i] = clusterValues[i][0] / clusterValues[i][1];
					unchanged = false;
				}
			}
		}

		if (unchanged) { // Check for no change in centers, indicating an end to the encapsulating while loop
			done = true;
		}

		for (int i = 0; i < numOfClusters; i++) {
			delete[] clusterValues[i];
		}
		delete[] clusterValues;
	}

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			kMeansImg.at<uchar>(i, j) = centers[pixelClusters[i * img.cols + j]];
		}
	}

	// Free array memory
	delete[] centers;
	delete[] pixelClusters;

	return kMeansImg;
}