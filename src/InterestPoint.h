#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class InterestPoint {

public:
	
	InterestPoint(void) {};
	InterestPoint(double s) { this->sigma = s; };
	~InterestPoint(void) {};

	double sigma;

	void run(Mat& in, vector<KeyPoint>& points);
	
	void showImage(Mat& img, const char* win, int wait, bool show, bool save);

	Mat createFstDevKernel(double sigma);

	void getInterestPoints(Mat& img, double sigma, vector<KeyPoint>& points);

	Mat nonMaxSuppression(Mat& img);

	
};
