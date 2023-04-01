#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core.hpp"
#include <string>

using namespace cv;
using namespace std;

bool cropBinImg(Mat binImg, float* bnds);
vector<Vec4i> compareDetectedLines(vector<Vec4i> aLines, vector<Vec4i> bLines);

int main(int argc, char** argv) {

	int vecDist = 15;
	int parallelDist = 0;

	if (argc != 4) {
		cout << "Invalid number of arguments! " << argc << "\n";
		exit(0);
	}

	vector<Mat> alphabetList;
	vector<vector<Vec4i>> alphabetLines;

	char alphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	for (int i = 0; i < sizeof(alphabet)/sizeof(char); i++) {
		char character = alphabet[i];
		string path = (string)"cw_" + character + ".png";
		Mat letter = imread(path, IMREAD_GRAYSCALE);

		Mat letterContours;
		vector<Vec4i> letterLines;

		threshold(letter, letter, 210, 255, THRESH_BINARY);
		Canny(letter, letterContours, 50, 250);
		HoughLinesP(letterContours, letterLines, 1, CV_PI / 180, 2, 1, 150);

		for (size_t i = 0; i < letterLines.size(); i++) {
			Vec4i lineOriginal = letterLines[i];
			int line_xMin = min(lineOriginal[0], lineOriginal[2]);
			int line_xMax = max(lineOriginal[0], lineOriginal[2]);
			int line_yMin = min(lineOriginal[1], lineOriginal[3]);
			int line_yMax = max(lineOriginal[1], lineOriginal[3]);

			Vec4i newLine = { line_xMin, line_yMin, line_xMax, line_yMax };
			letterLines.erase(letterLines.begin());
			letterLines.push_back(newLine);
		}

		alphabetList.push_back(letter);
		alphabetLines.push_back(letterLines);
	}

	Mat cwOriginal = imread(argv[1], IMREAD_COLOR);
	Mat cwCompleted = imread(argv[2], IMREAD_GRAYSCALE);
	Mat cwAlphabet = imread(argv[3], IMREAD_GRAYSCALE);

	Mat cw = cwOriginal.clone();
	Mat cwGrey = imread(argv[1], IMREAD_GRAYSCALE);
	imshow("Crossword Input Image", cw);
	waitKey(0);


	Mat cwBin;
	threshold(cwGrey, cwBin, 210, 255, THRESH_BINARY);
	imshow("Crossword Binarized", cwBin);
	imwrite("1_Crossword_Binarized.png", cwBin);
	waitKey(0);

	Mat cwBinNot = cwBin.clone();
	dilate(cwBinNot, cwBinNot, Mat::ones(3, 3, 0));
	bitwise_not(cwBinNot, cwBinNot);
	bitwise_or(cwBinNot, cwBin, cwBin);
	imshow("Crossword Binarized - After normalizing all cells", cwBin);
	imwrite("2_Crossword_Binarized_And_Normalized.png", cwBin);
	waitKey(0);

	float bnds[4];
	cropBinImg(cwBin, bnds);
	cwBin = cwBin(Range(bnds[0], bnds[2]), Range(bnds[1], bnds[3]));
	cw = cw(Range(bnds[0], bnds[2]), Range(bnds[1], bnds[3]));
	cwCompleted = cwCompleted(Range(bnds[0], bnds[2]), Range(bnds[1], bnds[3]));


	imshow("Cropped Binary", cwBin);
	imwrite("3_Crossword_Binarized_Cropped.png", cwBin);
	imshow("Cropped", cw);
	imwrite("4_Crossword_Original_Cropped.png", cw);
	waitKey(0);

	erode(cwBin, cwBin, Mat::ones(1, 1, 0));
	dilate(cwBin, cwBin, Mat::ones(1, 1, 0));
	imshow("Crossword Eroded", cwBin);
	waitKey(0);


	Mat contours;
	Canny(cwBin, contours, 50, 250);

	imshow("Canny Results", contours);
	imwrite("5_Crossword_Canny_Results.png", contours);
	waitKey(0);

	dilate(contours, contours, Mat::ones(3, 3, 0));
	imshow("Canny Results Dilated", contours);
	imwrite("6_Crossword_Canny_Results_Dilated.png", contours);
	waitKey(0);

	vector<Vec4i> lines, linesFinal;
	HoughLinesP(contours, lines, 1, CV_PI / 180, 300, 10, 150);

	vector<bool> lineIsMerged(lines.size());

	// Order line points to be (top-left-most point, bottom-right-most point
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i lineOriginal = lines[i];
		int line_xMin = min(lineOriginal[0], lineOriginal[2]);
		int line_xMax = max(lineOriginal[0], lineOriginal[2]);
		int line_yMin = min(lineOriginal[1], lineOriginal[3]);
		int line_yMax = max(lineOriginal[1], lineOriginal[3]);

		Vec4i newLine = { line_xMin, line_yMin, line_xMax, line_yMax };
		lines.erase(lines.begin());
		lines.push_back(newLine);
	}

	// Extraneous Line Removal Version 2 - Group all similar lines together and find average line

	for (size_t i = 0; i < lines.size(); i++) {
		if (!lineIsMerged[i]) {
			Vec4i aLine = lines[i];
			vector<Vec4i> aLineMatches;
			bool aLineIsHorizontal = false;
			bool aLineIsVertical = false;

			if (aLine[1] == aLine[3]) {
				aLineIsHorizontal = true;
			}
			else if (aLine[0] == aLine[2]) {
				aLineIsVertical = true;
			}

			aLineMatches.push_back(aLine);

			for (size_t j = 0; j < lines.size(); j++) {
				if (i != j && !lineIsMerged[i]) {

					Vec4i bLine = lines[j];
					if (aLine == bLine) {
						aLineMatches.push_back(bLine);
					}
					// If the 2 lines are parallel and horizontal
					else if (abs(bLine[0] - aLine[0]) <= parallelDist &&
						abs(bLine[2] - aLine[2]) <= parallelDist &&
						(abs(bLine[1] - aLine[1]) <= vecDist || abs(bLine[3] - aLine[3]) <= vecDist)
						) {
						aLineMatches.push_back(bLine);
						lineIsMerged[j] = true;
					}
					// If the 2 lines are parallel and vertical
					else if (abs(bLine[1] - aLine[1]) <= parallelDist &&
						abs(bLine[3] - aLine[3]) <= parallelDist &&
						(abs(bLine[0] - aLine[0]) <= vecDist || abs(bLine[2] - aLine[2]) <= vecDist)
						) {
						aLineMatches.push_back(bLine);
						lineIsMerged[j] = true;
					}
				}
			}

			if (aLineIsHorizontal) {
				int n0, n1, n2, n3;
				n0 = n1 = n3 = 0;
				n2 = cw.cols - 1;

				for (size_t i = 0; i < aLineMatches.size(); i++) {
					Vec4i matchLine = aLineMatches[i];
					n1 += matchLine[1];
					n3 += matchLine[3];
				}
				n1 = n1 / aLineMatches.size();
				n3 = n3 / aLineMatches.size();
				Vec4i newLine = Vec4i(n0, n1, n2, n3);
				linesFinal.push_back(newLine);
				lineIsMerged[i] = true;
			}
			else if (aLineIsVertical) {
				int n0, n1, n2, n3;
				n0 = n1 = n2 = 0;
				n3 = cw.rows - 1;

				for (size_t i = 0; i < aLineMatches.size(); i++) {
					Vec4i matchLine = aLineMatches[i];
					n0 += matchLine[0];
					n2 += matchLine[2];
				}
				n0 = n0 / aLineMatches.size();
				n2 = n2 / aLineMatches.size();
				Vec4i newLine = Vec4i(n0, n1, n2, n3);
				linesFinal.push_back(newLine);
				lineIsMerged[i] = true;
			}


			if (!lineIsMerged[i]) {
				linesFinal.push_back(aLine);
				lineIsMerged[i] = true;
			}
		}
	}


	// Extraneous Line Removal Version 1 - Problem: doesn't remove all extraneous lines

	//for (size_t i = 0; i < lines.size(); i++) {
	//	Vec4i aLine = lines[i];

	//	for (size_t j = 0; j < lines.size(); j++) {
	//		if (i != j && !lineIsMerged[i]) {
	//			
	//			Vec4i bLine = lines[j];
	//			if (aLine == bLine) {
	//				lineIsMerged[i] = true;
	//				lineIsMerged[j] = true;
	//				linesFinal.push_back(aLine);
	//			}
	//			// If the 2 lines are parallel and horizontal
	//			else if (abs(bLine[0] - aLine[0]) <= parallelDist &&
	//				abs(bLine[2] - aLine[2]) <= parallelDist &&
	//				(abs(bLine[1] - aLine[1]) <= vecDist || abs(bLine[3] - aLine[3]) <= vecDist)
	//				) {
	//				//if (abs(bLine[0] - aLine[0]) <= vecDist && abs(bLine[1] - aLine[1]) <= vecDist && abs(bLine[2] - aLine[2]) <= vecDist && abs(bLine[3] - aLine[3]) <= vecDist){
	//				lineIsMerged[i] = true;
	//				lineIsMerged[j] = true;

	//				int n0 = 0;
	//				int n1 = (bLine[1] + aLine[1]) / 2;
	//				int n2 = cw.cols - 1;
	//				int n3 = (bLine[3] + aLine[3]) / 2;
	//				Vec4i newLine = Vec4i(n0, n1, n2, n3);
	//				linesFinal.push_back(newLine);
	//			} 
	//			else if (abs(bLine[1] - aLine[1]) <= parallelDist &&
	//				abs(bLine[3] - aLine[3]) <= parallelDist &&
	//				(abs(bLine[0] - aLine[0]) <= vecDist || abs(bLine[2] - aLine[2]) <= vecDist) &&
	//				!lineIsMerged[i]) {
	//				// If the 2 lines are parallel and vertical
	//				lineIsMerged[i] = true;
	//				lineIsMerged[j] = true;

	//				int n0 = (bLine[0] + aLine[0]) / 2;
	//				int n1 = 0;
	//				int n2 = (bLine[2] + aLine[2]) / 2;
	//				int n3 = cw.rows - 1;
	//				Vec4i newLine = Vec4i(n0, n1, n2, n3);
	//				linesFinal.push_back(newLine);
	//			}
	//		}
	//	}

	//	if (!lineIsMerged[i]) {
	//		linesFinal.push_back(aLine);
	//		lineIsMerged[i] = true;
	//	}
	//}

	Mat cw2 = cw.clone();

	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(cw, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 1, LINE_AA);
	}
	for (size_t i = 0; i < linesFinal.size(); i++) {
		Vec4i l = linesFinal[i];
		line(cw2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 1, LINE_AA);
	}

	imshow("Crossword Line Detection", cw);
	imwrite("7_Crossword_Line_Detection_Extraneous.png", cw);
	imshow("Crossword Line Detection v2", cw2);
	imwrite("8_Crossword_Line_Detection_Extraneous-Lines-Removed.png", cw2);
	waitKey(0);

	threshold(cw2, cw2, 128, 255, THRESH_BINARY);
	cvtColor(cw2, cw2, COLOR_BGR2GRAY);
	imshow("cw2 before labels", cw2);
	waitKey(0);
	Mat labels(cw2.size(), CV_32S);
	int cc = connectedComponents(cw2, labels, 8, CV_32S);
	cout << "\nConnected Components: " << "\n" << cc;

	int* regionAreas = new int[cc];
	for (int i = 0; i < cc; i++) {
		regionAreas[i] = 0;
	}

	for (int i = 0; i < labels.rows; i++) {
		for (int j = 0; j < labels.cols; j++) {
			int label = labels.at<int>(i, j);
			regionAreas[label] += 1;
		}
	}

	vector<Mat> cellList;
	for (int k = 1; k < cc; k++) {
		Mat cell = Mat::zeros(cw2.rows, cw2.cols, CV_8UC1);

		for (int i = 0; i < cwCompleted.rows; i++) {
			for (int j = 0; j < cwCompleted.cols; j++) {

				if (labels.at<int>(i, j) == k) {
					cell.at<uchar>(i, j) = cwCompleted.at<uchar>(i,j);
				}
				else {
					cell.at<uchar>(i, j) = 0;
				}
			}
		}
		cell = cell(Range(1, cell.rows - 1), Range(1, cell.cols - 1));
		threshold(cell, cell, 192, 255, THRESH_BINARY);
		bitwise_not(cell, cell);
		float cellBnds[4];
		bool didCrop = cropBinImg(cell, cellBnds);
		if (didCrop) {
			bitwise_not(cell, cell);
			cell = cell(Range(cellBnds[0], cellBnds[2]), Range(cellBnds[1], cellBnds[3]));
			cellList.push_back(cell);
			imwrite((string)"Cell" + to_string(k) + ".png", cell);
		}
		
	}

	
	for (int i = 0; i < cellList.size(); i++) {
		Mat cell = cellList[i];

		Mat cellContours;
		Canny(cell, cellContours, 50, 250);
		vector<Vec4i> cellLines;
		HoughLinesP(cellContours, cellLines, 1, CV_PI / 180, 2, 1, 150);

		imshow("Cell Canny", cellContours);
		waitKey(0);

		for (size_t j = 0; j < cellLines.size(); j++) {
			Vec4i lineOriginal = cellLines[j];
			int line_xMin = min(lineOriginal[0], lineOriginal[2]);
			int line_xMax = max(lineOriginal[0], lineOriginal[2]);
			int line_yMin = min(lineOriginal[1], lineOriginal[3]);
			int line_yMax = max(lineOriginal[1], lineOriginal[3]);

			Vec4i newLine = { line_xMin, line_yMin, line_xMax, line_yMax };
			cellLines.erase(cellLines.begin());
			cellLines.push_back(newLine);
		}

		Mat cellWithLines = cell.clone();
		cvtColor(cellWithLines, cellWithLines, COLOR_GRAY2BGR);

		

		for (int j = 0; j < alphabetList.size(); j++) {

			Mat letter = alphabetList[j];
			vector<Vec4i> matches = compareDetectedLines(cellLines, alphabetLines[j]);

			if (matches.size() >= 0) {
				// Match digital letter to physical letter
			}

			Mat letterWithLines = letter.clone();
			cvtColor(letterWithLines, letterWithLines, COLOR_GRAY2BGR);

			imshow("Comparison Letter", letterWithLines);
			imshow("Comparison Cell", cellWithLines);
			waitKey(0);
		}
	}


	return 0;
}

bool cropBinImg(Mat binImg, float* bnds) {
	bool point1Found = false;
	float x1, y1, x2, y2;
	x1 = y1 = x2 = y2 = 0;

	for (int i = 0; i < binImg.rows; i++) {
		for (int j = 0; j < binImg.cols; j++) {

			if (binImg.at<uchar>(i, j) == 0) {

				if (!point1Found) {
					x1 = i;
					y1 = j;
					point1Found = true;
				}

				if (i > x2) {
					x2 = i;
				}

				if (j > y2) {
					y2 = j;
				}
			}
		}
	}

	if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0) {
		return false;

	}

	bnds[0] = x1 + 1;
	bnds[1] = y1 + 1;
	bnds[2] = x2 - 1;
	bnds[3] = y2 - 1;
	return true;
}

vector<Vec4i> compareDetectedLines(vector<Vec4i> aLines, vector<Vec4i> bLines) {
	vector<Vec4i> matches;

	int minVectorSize = min(aLines.size(), bLines.size());
	for (int i = 0; i < minVectorSize; i++) {
		Vec4i aLine = aLines[i];
		Vec4i bLine = bLines[i];

		int aLineXDiff = aLine[2] - aLine[0];
		int aLineYDiff = aLine[3] - aLine[1];
		int bLineXDiff = bLine[2] - bLine[0];
		int bLineYDiff = bLine[3] - bLine[1];

		float aLineSlope = 0, bLineSlope = 0;
		if (aLineXDiff == 0) {
			aLineSlope = -1;
		}
		else {
			aLineSlope = aLineYDiff / aLineXDiff;
		}

		if (bLineXDiff == 0) {
			bLineSlope = -1;
		}
		else {
			bLineSlope = bLineYDiff / bLineXDiff;
		}

		if (abs(aLineXDiff - bLineXDiff) <= 2 && 
			abs(aLineYDiff - bLineYDiff) <= 2 && 
			abs(aLineSlope - bLineSlope) <= 1) {
			matches.push_back(aLine);
		}
	}

	return matches;
}

