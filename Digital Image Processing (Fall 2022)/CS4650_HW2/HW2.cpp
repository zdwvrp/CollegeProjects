#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <list>
#include <chrono>

using namespace std::chrono;
using namespace cv;
using namespace std;

Mat AdaptiveMedian(int Smax, Mat img);

int main(int argc, char** argv) {

	if (argc != 3) {
		cout << "Invalid number of arguments! " << argc << "\n";
		exit(0);
	}

	Mat ABQ = imread(argv[1], IMREAD_GRAYSCALE);
	Mat Peppers = imread(argv[2], IMREAD_GRAYSCALE);

	imshow("Noisy Peppers Image", Peppers);
	waitKey(0);

	auto start = high_resolution_clock::now();

	Mat peppers_9 = AdaptiveMedian(9, Peppers);
	imwrite("peppers_smax9.png", peppers_9);
	imshow("Peppers with Smax = 9", peppers_9);
	waitKey(0);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << "\nPeppers Smax = 9 Completion Time: " << duration.count() << " seconds.";
	start = high_resolution_clock::now();

	Mat peppers_21 = AdaptiveMedian(21, Peppers);

	stop = high_resolution_clock::now();
	duration = duration_cast<seconds>(stop - start);
	cout << "\nPeppers Smax = 21 Completion Time: " << duration.count() << " seconds.";
	start = high_resolution_clock::now();

	imwrite("peppers_smax21.png", peppers_21);
	imshow("Peppers with Smax = 21", peppers_21);
	waitKey(0);

	Mat abq_9 = AdaptiveMedian(9, ABQ);

	stop = high_resolution_clock::now();
	duration = duration_cast<seconds>(stop - start);
	cout << "\nABQ Smax = 9 Completion Time: " << duration.count() << " seconds.";
	start = high_resolution_clock::now();

	imwrite("abq_smax9.png", abq_9);
	imshow("ABQ with Smax = 9", abq_9);
	waitKey(0);

	Mat abq_21 = AdaptiveMedian(21, ABQ);

	stop = high_resolution_clock::now();
	duration = duration_cast<seconds>(stop - start);
	cout << "\nABQ Smax = 21 Completion Time: " << duration.count() << " seconds.";
	start = high_resolution_clock::now();

	imwrite("abq_smax21.png", abq_21);
	imshow("ABQ with Smax = 21", abq_21);
	waitKey(0);

	return 0;
}

Mat AdaptiveMedian(int smax, Mat img) {
	Mat imgClone = img.clone();
	int zmin, zmax, zmed, zxy;
	int windowSize;
	bool* finalPixels = new bool[img.rows * img.cols]();

	// Loop through every pixel in the image
	// i = current row
	// j = current column
	for (windowSize = 3; windowSize <= smax; windowSize += 2) {

		list<int> windowPixels;
		cout << "Now running through image with a window size " << windowSize << "\n";

		for (int i = 0; i < img.rows; i++) {			
			int distFromLastValidPix = 0; // Used to keep track of how far the window has traveled since the last un-finalized pixel. Resets on each new row.


			for (int j = 0; j < img.cols; j++) {

				if (windowSize == 3) { // If it's the first run through the image, initialize all pixels to false, since none have been finalized yet.
					finalPixels[i * img.cols + j] = false;
				}

				if (!finalPixels[i * img.cols + j]) { // Only runs expensive code segment if the current pixel hasn't been set yet.
					

					/* Calculate zmed, zmin and zmax */

					zxy = img.at<uchar>(i, j); // Value for Zxy, or the current pixel.
					int radius = windowSize / 2; // Radius is the number of pixels that exist, in a straight line, between the center and the edge of the window.

					if (j == 0 || distFromLastValidPix > 0) { // If true, then the window must be cleared and remade, since none of the previous values can be used.

						windowPixels.clear();

						// Populate the windowPixels list with the values of the current window.
						for (int winRow = i - radius; winRow <= i + radius; winRow++) {
							for (int winCol = j - radius; winCol <= j + radius; winCol++) {

								if (winRow >= 0 && winRow < img.rows && winCol >= 0 && winCol < img.cols) { // Ensures all entries to windowPixels are in bounds.
									windowPixels.push_back((int)img.at<uchar>(winRow, winCol));
								}
							}
						}
					}
					else { // The current pixel is neighboring the previous pixel, and shared neighbor values are kept.

						for (int l = 0; l < windowSize; l++) {

							if (i - radius + l >= 0 && i + radius + l < img.rows && j - radius - 1 >= 0) {
								windowPixels.pop_front(); // Remove pixel from previous window
							}

							if (i - radius + l >= 0 && i + radius + l < img.rows && j + radius < img.cols) {
								windowPixels.push_back((int)img.at<uchar>((i - radius + l), j + radius)); // Add pixel to new window
							}
						}
					}


				

					// Sort pixel values and store in a separate list.
					list<int> sortedWinPix = windowPixels;
					sortedWinPix.sort();

					// Set zmin, zmax and zmed values
					zmin = sortedWinPix.front();
					zmax = sortedWinPix.back();

					auto winPixPtr = sortedWinPix.begin();
					advance(winPixPtr, (sortedWinPix.size() / 2) - 1);
					zmed = *winPixPtr;

					// Adaptive Median Filter Logic
					if (zmed > zmin && zmax > zmed) {
						if (zxy > zmin && zmax > zxy) {
							imgClone.at<uchar>(i, j) = zxy;
						}
						else {
							imgClone.at<uchar>(i, j) = zmed;
						}

						finalPixels[i * img.cols + j] = true; // Since the pixel value has been changed, it is now considered finalized.
					}


					if (windowSize == smax && !finalPixels[i * img.cols + j]) {
						imgClone.at<uchar>(i, j) = zmed;
						finalPixels[i * img.cols + j] = true; // Since the pixel value has been changed, it is now considered finalized.
					}

					distFromLastValidPix = 0; // Distance is reset to zero, as a valid pixel has just been analyzed.
				}

				// If the current pixel has already been finalized, the distance var is incremented, to show that the window will need to be cleared.
				else {
					distFromLastValidPix++;
				}
			}
		}
	}
	return imgClone;
}