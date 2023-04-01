#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat Translate(Mat img, float tx, float ty);
Mat CropScale(Mat img, float x1, float x2, float y1, float y2, float scale);
Mat VerticalFlip(Mat img);
Mat HorizontalFlip(Mat img);
Mat Rotate(Mat img, float angle);
Mat Fill(Mat img, int x1, int x2, int y1, int y2, uchar val);

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "Invalid number of arguments! " << argc << "\n";
		exit(0);
	}

	Mat nakaInput = imread(argv[1], IMREAD_GRAYSCALE);

	imshow("Naka Input Image", nakaInput);
	waitKey(0);


	//Mat nakaTranslated = Translate(nakaInput, 50, 100);
	//imshow("Naka Translated", nakaTranslated);
	//imwrite("nakaTranslated.png", nakaTranslated);
	//waitKey(0);


	//Mat nakaCropScaled = CropScale(nakaInput, 1, 100, 1, 200, 2);
	//imshow("Naka Cropped and Scaled", nakaCropScaled);
	//imwrite("nakaCropScaled.png", nakaCropScaled);
	//waitKey(0);
	//

	//Mat nakaVFlipped = VerticalFlip(nakaInput);
	//imshow("Naka Flipped Vertically", nakaVFlipped);
	//imwrite("nakaVertFlipped.png", nakaVFlipped);
	//waitKey(0);


	//Mat nakaHFlipped = HorizontalFlip(nakaInput);
	//imshow("Naka Flipped Horizontally", nakaHFlipped);
	//imwrite("nakaHoriFlipped.png", nakaHFlipped);
	//waitKey(0);


	//Mat nakaRotated = Rotate(nakaInput, 30);
	//imshow("Naka Rotated", nakaRotated);
	//imwrite("nakaRotated.png", nakaRotated);
	//waitKey(0);


	//Mat nakaFilled = Fill(nakaInput, 100, 300, 50, 400, 150);
	//imshow("Naka Filled", nakaFilled);
	//imwrite("nakaFilled.png", nakaFilled);
	//waitKey(0);


	Mat xGradient;
	Mat yGradient;
	Sobel(nakaInput, xGradient, CV_32F, 1, 0, 11);
	Sobel(nakaInput, yGradient, CV_32F, 0, 1, 11);

	imshow("Naka X Gradient Large Scale", xGradient);
	imwrite("nakaXGradientLarge.png", xGradient);
	imshow("Naka Y Gradient Large Scale", yGradient);
	imwrite("nakaYGradientLarge.png", yGradient);
	waitKey(0);

	Mat nakaMagnitude;
	Mat nakaOrientation;
	cartToPolar(xGradient, yGradient, nakaMagnitude, nakaOrientation, 1);

	imshow("Naka Magnitude Large Scale", nakaMagnitude);
	imwrite("nakaMagnitudeLarge.png", nakaMagnitude);
	imshow("Naka Orientation Large Scale", nakaOrientation);
	imwrite("nakaOrientationLarge.png", nakaOrientation);
	waitKey(0);
	
	int* binValues = new int[360]();

	for (int i = 0; i < 360; i++) {
		binValues[i] = 0;
	}

	for (int i = 0; i < nakaMagnitude.rows; i++) {
		for (int j = 0; j < nakaMagnitude.cols; j++) {
			int angle = (int)nakaOrientation.at<float>(i, j);
			binValues[angle] ++;
		}
	}
	Mat histogram(1000, 720, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < 359; i++) {
		Point p1((i - 1) * 2, 999 - (binValues[i - 1] / 100));
		Point p2((i + 1) * 2, 999 - (binValues[i + 1] / 100));

		line(histogram, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Generated Histogram Large Scale", histogram);
	imwrite("histogramLarge.png", histogram);
	waitKey(0);


	Sobel(nakaInput, xGradient, CV_32F, 1, 0, 3);
	Sobel(nakaInput, yGradient, CV_32F, 0, 1, 3);

	imshow("Naka X Gradient Small Scale", xGradient);
	imwrite("nakaXGradientSmall.png", xGradient);
	imshow("Naka Y Gradient Small Scale", yGradient);
	imwrite("nakaYGradientSmall.png", yGradient);
	waitKey(0);

	cartToPolar(xGradient, yGradient, nakaMagnitude, nakaOrientation, 1);

	imshow("Naka Magnitude Small Scale", nakaMagnitude);
	imwrite("nakaMagnitudeSmall.png", nakaMagnitude);
	imshow("Naka Orientation Small Scale", nakaOrientation);
	imwrite("nakaOrientationSmall.png", nakaOrientation);
	waitKey(0);

	int* binValuesS = new int[360]();

	for (int i = 0; i < 360; i++) {
		binValuesS[i] = 0;
	}

	for (int i = 0; i < nakaMagnitude.rows; i++) {
		for (int j = 0; j < nakaMagnitude.cols; j++) {
			int angle = (int)nakaOrientation.at<float>(i, j);
			binValuesS[angle] ++;
		}
	}
	Mat histogramS(1000, 720, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < 360; i++) {
		Point p1((i - 1) * 2, 999 - (binValuesS[i - 1] / 100));
		Point p2((i + 1) * 2, 999 - (binValuesS[i + 1] / 100));

		line(histogramS, p1, p2, Scalar(255, 255, 255), 1, LINE_8);
	}
	imshow("Generated Histogram Small Scale", histogramS);
	imwrite("histogramSmall.png", histogramS);
	waitKey(0);

	return 0;
}

Mat Translate(Mat img, float tx, float ty) {
	Mat imgTranslated;
	float matrix[] = { 1, 0, tx, 0, 1, ty };
	Mat matrixMat = Mat(2, 3, CV_32F, matrix);
	
	warpAffine(img, imgTranslated, matrixMat, img.size());
	return imgTranslated;
}


Mat CropScale(Mat img, float x1, float x2, float y1, float y2, float scale) {
	Mat imgCropScaled;
	imgCropScaled = img(Range(x1, x2), Range(y1, y2));

	resize(imgCropScaled, imgCropScaled, Size(), scale, scale);
	return imgCropScaled;
}
Mat VerticalFlip(Mat img) {
	Mat imgVertFlipped;
	flip(img, imgVertFlipped, 0);
	return imgVertFlipped;
}
Mat HorizontalFlip(Mat img) {
	Mat imgHorFlipped;
	flip(img, imgHorFlipped, 1);
	return imgHorFlipped;
}
Mat Rotate(Mat img, float angle) {
	Mat imgRotated;
	Point2f center( (img.cols - 1) / 2.0, (img.rows - 1) / 2.0 );
	Mat matrixMat = getRotationMatrix2D(center, angle, 1);

	warpAffine( img, imgRotated, matrixMat, img.size() );
	return imgRotated;
}
Mat Fill(Mat img, int x1, int x2, int y1, int y2, uchar val) {
	Mat imgFilled = img.clone();

	for (int i = x1; i < x2; i++) {
		for (int j = y1; j < y2; j++) {
			imgFilled.at<uchar>(i, j) = val;
		}
	}
	return imgFilled;
}