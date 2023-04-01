#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if (argc != 4) {
		cout << "Invalid number of arguments!" << "\n";
		exit(0);
	}

	uchar threshold = atoi(argv[3]);
	cout << threshold;
	Mat img = imread(argv[1], IMREAD_GRAYSCALE);

	if (img.empty()) {
		cout << "Image not found!" << "\n";
		exit(0);
	}

	imshow("Grayscale Image", img);
	waitKey(0);

	int i, j;
	uchar* cells = new uchar[img.rows * img.cols];
	for (i = 0; i < img.rows; i++) {
		for (j = 0; j < img.cols; j++) {

			uchar cellColor = img.at<uchar>(i, j);
			if (cellColor >= threshold) {
				cells[i * img.rows + j] = 255;
			}
			else {
				cells[i * img.rows + j] = 0;
			}
		}
	}
	imshow("Computed Binarized Image", Mat(img.cols, img.rows, CV_8UC1, cells));
	waitKey(0);
	imwrite(argv[2], Mat(img.cols, img.rows, CV_8UC1, cells)); 

	return 0;
}