#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <tgmath.h>

using namespace cv;
using namespace std;

void myimhist(int nbins, Mat img);
void myimhistmask(int nbins, Mat img, Mat mask);
void linearStretch(int nbins, Mat image);
void linearStretchDiscard(int nbins, float discard, Mat img);

int main(int argc, char** argv) {

	if (argc != 4) {
		cout << "Invalid number of arguments!" << "\n";
		exit(0);
	}

	Mat img = imread(argv[1]);
	Mat mask = imread(argv[2]);
	Mat stretch = imread(argv[3], IMREAD_GRAYSCALE);

	if (img.empty() || mask.empty()) {
		cout << "Image(s) not found!" << "\n";
		exit(0);
	}

	int nbins;
	imshow("Grayscale Image", img);
	waitKey(0);

	cout << "\nB1 Part 1 - Histogram";
	cout << "\nPlease enter your desired number of bins: ";
	cin >> nbins;
	cout << "\nSetting number of bins to " << nbins << "\n";

	myimhist(nbins, img);

	cout << "\nB1 Part 2 - Histogram using Binary Mask";
	cout << "\nPlease enter your desired number of bins: ";
	cin >> nbins;
	cout << "\nSetting number of bins to " << nbins << "\n";

	myimhistmask(nbins, img, mask);

	cout << "\nB2 - Min-Max Linear Contrast Stretch";

	linearStretch(256, stretch);
	linearStretchDiscard(256, .01, stretch);
	linearStretchDiscard(256, .05, stretch);
	
	return 0;
}

void myimhist(int nbins, Mat img) {
	int rows = (int)img.rows;
	int cols = (int)img.cols;
	int bitsPerBin = 256 / nbins;
	int* binValues = new int[nbins]();
	int* quantizedImage = new int[rows * cols]();
	

	// Populates the histogram array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int cellColor = img.at<uchar>(i, j);
			quantizedImage[i * img.rows + j] = cellColor / bitsPerBin;
		}
	}
	int maxValue = binValues[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		binValues[temp] += 1;

		if (binValues[temp] > maxValue) {
			maxValue = binValues[temp];
		}
	}

	int xBound = nbins * bitsPerBin * 2 + 2;
	int yBound = maxValue/100 + 10;
	Mat histogram(yBound, xBound, CV_8UC3, Scalar(0,0,0));
	
	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValues[(l - 1)]/100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValues[l]/100));


		line(histogram, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Generated Histogram", histogram);
	waitKey(0);
}

void myimhistmask(int nbins, Mat img, Mat mask) {
	int rows = (int)img.rows;
	int cols = (int)img.cols;
	int bitsPerBin = 256 / nbins;
	int* binValues = new int[nbins]();
	int* quantizedImage = new int[rows * cols]();


	// Populates the histogram array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int maskColor = mask.at<uchar>(i, j);
			if (maskColor == 255) {
				int cellColor = img.at<uchar>(i, j);
				quantizedImage[i * rows + j] = cellColor / bitsPerBin;
			}
			else {
				quantizedImage[i * rows + j] = -1;
			}
		}
	}
	int maxValue = binValues[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		if (temp > -1) {
			binValues[temp] += 1;

			if (binValues[temp] > maxValue) {
				maxValue = binValues[temp];
			}
		}
	}

	int xBound = nbins * bitsPerBin * 2 + 2;
	int yBound = maxValue / 100 + 10;
	Mat histogram(yBound, xBound, CV_8UC3, Scalar(0, 0, 0));

	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValues[(l - 1)] / 100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValues[l] / 100));


		line(histogram, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Generated Histogram using Mask", histogram);
	waitKey(0);
}

void linearStretch(int nbins, Mat image) {
	Mat img = image.clone();
	int rows = (int)img.rows;
	int cols = (int)img.cols;
	int bitsPerBin = 256 / nbins;
	int* binValues = new int[nbins]();
	int* binValuesStretch = new int[nbins]();
	int* quantizedImage = new int[rows * cols]();

	Mat stretchImg = img.clone();

	// Populates the histogram array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int cellColor = img.at<uchar>(i, j);
			quantizedImage[i * rows + j] = cellColor / bitsPerBin;
		}
	}
	int maxValue = binValues[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		binValues[temp] += 1;

		if (binValues[temp] > maxValue) {
			maxValue = binValues[temp];
		}
	}

	int xBound = nbins * bitsPerBin * 2 + 2;
	int yBound = maxValue / 500 + 10;
	Mat histogram(yBound, xBound, CV_8UC3, Scalar(0, 0, 0));

	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValues[(l - 1)] / 100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValues[l] / 100));


		line(histogram, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Original Image", img);
	imshow("Generated Histogram for Original Image", histogram);
	waitKey(0);

	int min = 0;
	int max = 256;
	for (int m = 0; m < nbins; m++) {
		if (binValues[m] > 0) {
			min = m;
			m = nbins;
		}
	}

	for (int n = nbins - 1; n >= 0; n--) {
		if (binValues[n] > 0) {
			max = n;
			n = -1;
		}
	}

	for (int p = 0; p < stretchImg.rows; p++) {
		for (int q = 0; q < cols; q++) {
			float cellColor = stretchImg.at<uchar>(p, q);
			cellColor = round( 255 * ((cellColor - min) / (max - min)) );
			stretchImg.at<uchar>(p, q) = (int)cellColor;
		}
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int cellColor = stretchImg.at<uchar>(i, j);
			quantizedImage[i * rows + j] = cellColor / bitsPerBin;
		}
	}
	maxValue = binValuesStretch[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		binValuesStretch[temp] += 1;

		if (binValues[temp] > maxValue) {
			maxValue = binValuesStretch[temp];
		}
	}

	Mat histogramStretch(yBound, xBound, CV_8UC3, Scalar(0, 0, 0));

	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValuesStretch[(l - 1)] / 100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValuesStretch[l] / 100));


		line(histogramStretch, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Stretched Image", stretchImg);
	imshow("Generated Histogram for Stretched Image", histogramStretch);
	waitKey(0);
}

void linearStretchDiscard(int nbins, float discard, Mat image) {
	Mat img = image.clone();
	int rows = (int)img.rows;
	int cols = (int)img.cols;
	int bitsPerBin = 256 / nbins;
	int* binValues = new int[nbins]();
	int* binValuesStretch = new int[nbins]();
	int* quantizedImage = new int[rows * cols]();

	Mat stretchImg = image.clone();

	// Populates the histogram array
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int cellColor = img.at<uchar>(i, j);
			quantizedImage[i * rows + j] = cellColor / bitsPerBin;
		}
	}
	int maxValue = binValues[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		binValues[temp] += 1;

		if (binValues[temp] > maxValue) {
			maxValue = binValues[temp];
		}
	}

	int xBound = nbins * bitsPerBin * 2 + 2;
	int yBound = maxValue / 500 + 10;
	Mat histogram(yBound, xBound, CV_8UC3, Scalar(0, 0, 0));

	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValues[(l - 1)] / 100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValues[l] / 100));

		line(histogram, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Original Image", img);
	imshow("Generated Histogram for Original Image", histogram);
	waitKey(0);

	int min = 0;
	int max = 256;
	for (int m = 0; m < nbins; m++) {
		if (binValues[m] > 0) {
			min = m;
			m = nbins;
		}
	}
	for (int n = nbins - 1; n >= 0; n--) {
		if (binValues[n] > 0) {
			max = n;
			n = -1;
		}
	}
	min += round((float)(max - min)* discard);
	max -= round((float)(max - min)* discard);

	for (int p = 0; p < stretchImg.rows; p++) {
		for (int q = 0; q < cols; q++) {
			float cellColor = stretchImg.at<uchar>(p, q);
			cellColor = round(255 * ((cellColor - min) / (max - min)));
			if (cellColor < 0) {
				cellColor = 0;
			}
			stretchImg.at<uchar>(p, q) = (int)cellColor;
		}
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int cellColor = stretchImg.at<uchar>(i, j);
			quantizedImage[i * rows + j] = cellColor / bitsPerBin;
		}
	}
	maxValue = binValuesStretch[0];
	for (int k = 0; k < (rows * cols); k++) {
		int temp = quantizedImage[k];
		binValuesStretch[temp] += 1;

		if (binValues[temp] > maxValue) {
			maxValue = binValuesStretch[temp];
		}
	}

	Mat histogramStretch(yBound, xBound, CV_8UC3, Scalar(0, 0, 0));

	for (int l = 1; l < nbins; l++) {
		Point p1((l - 1) * bitsPerBin * 2, yBound - 1 - (binValuesStretch[(l - 1)] / 100));
		Point p2(l * bitsPerBin * 2, yBound - 1 - (binValuesStretch[l] / 100));


		line(histogramStretch, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Stretched Image", stretchImg);
	imshow("Generated Histogram for Stretched Image", histogramStretch);
	waitKey(0);
}