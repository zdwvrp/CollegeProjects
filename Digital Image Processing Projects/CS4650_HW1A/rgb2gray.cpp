#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if ( argc != 3 ) {
		cout << "Invalid number of arguments!" << "\n";
		exit(0);
	}

	Mat img = imread( argv[1], IMREAD_COLOR );
	
	if ( img.empty() ) {
		cout << "Image not found!" << "\n";
		exit(0);
	}

	imshow("RGB Image", img);
	waitKey(0);

	//Gray[x][y]= 0.2989 * Red[x][y] + 0.5871 * Green[x][y] + 0.1140 * Blue[x][y]
	int i,j;
	uchar* cells = new uchar[img.rows * img.cols];
	for ( i = 0; i < img.rows; i++ ) {
		for ( j = 0; j < img.cols; j++ ) {

			Vec3b cellColor = img.at<Vec3b>(i, j);
			cells[i * img.rows + j] = 0.2989 * cellColor.val[0] + 0.5871 * cellColor.val[1] + 0.1140 * cellColor.val[2];
		}
	}
	imshow("Computed Grayscale Image", Mat(img.cols, img.rows, CV_8UC1, cells));
	waitKey(0);
	imwrite(argv[2], Mat(img.cols, img.rows, CV_8UC1, cells));


	return 0;

}