
#include "InterestPoint.h"

// Förstner Operator
void InterestPoint::getInterestPoints(Mat& img, double sigma, vector<KeyPoint>& points) {

	Mat Kernel_x = createFstDevKernel(sigma);
	Mat Kernel_y;
	transpose(Kernel_x, Kernel_y);
	// Mat gradient_y = gradient_x.t();

	// Convolution with filter Kernel
	Mat g_x(img.size(), img.type());
	filter2D(img, g_x, -1, Kernel_x);

	Mat g_y(img.size(), img.type());
	filter2D(img, g_y, -1, Kernel_y);

	// Multiplication of g_x, g_y
	Mat GxGx(img.size(), img.type());
	multiply(g_x, g_x, GxGx);

	Mat GyGy(img.size(), img.type());
	multiply(g_y, g_y, GyGy);

	Mat GxGy(img.size(), img.type());
	multiply(g_x, g_y, GxGy);

	// Gaussian Blurring 
	int k_avg = 5;
	double sigma_avg = 0.3*((k_avg - 1)*0.5 - 1) + 0.8;
	Mat GxGx_blur(img.size(), img.type());
	GaussianBlur(GxGx, GxGx_blur, Size(k_avg, k_avg), sigma_avg);

	Mat GyGy_blur(img.size(), img.type());
	GaussianBlur(GyGy, GyGy_blur, Size(k_avg, k_avg), sigma_avg);

	Mat GxGy_blur(img.size(), img.type());
	GaussianBlur(GxGy, GxGy_blur, Size(k_avg, k_avg), sigma_avg);

	// Trace and Determinant 
	Mat trace = GxGx_blur + GyGy_blur;

	Mat det_GxGx_GyGy_blur(img.size(), img.type());
	multiply(GxGx_blur, GyGy_blur, det_GxGx_GyGy_blur);

	Mat det_GxGy_blur(img.size(), img.type());
	multiply(GxGy_blur, GxGy_blur, det_GxGy_blur);

	Mat det = det_GxGx_GyGy_blur + det_GxGy_blur;

	// weight, max suppression and threshold
	Mat w(img.size(), img.type());
	divide(det, trace, w);

	Mat w_suppr = nonMaxSuppression(w);

	Scalar w_mean = mean(w_suppr);
	float w_Mean = w_mean.val[0];

	float w_min = 1 * w_Mean;

	// isotropy, max suppression and threshold
	Mat trace_2(img.size(), img.type());
	multiply(trace, trace, trace_2);

	Mat q(img.size(), img.type());
	divide(4 * det, trace_2, q);

	Mat q_suppr = nonMaxSuppression(q);

	double q_min = 0.65;

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if ((w_suppr.at<float>(i, j)> w_min) && (q_suppr.at<float>(i, j)> q_min)) {
				KeyPoint point = KeyPoint();
				point.pt.x = j;
				point.pt.y = i;

				point.size = 4.0;

				points.push_back(point);
			}
		}
	}

}

// creates kernel representing fst derivative of a Gaussian kernel in x-direction
Mat InterestPoint::createFstDevKernel(double sigma) {
	
	int kernelSize = 3;
	Mat Kernel(kernelSize, kernelSize, CV_32F);
	int mu = (kernelSize - 1) / 2;

	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			Kernel.at<float>(j, i) = -(j - mu)* exp(-((i - mu)*(i - mu) + (j - mu)*(j - mu)) / (2 * sigma*sigma));
		}
	}

	return Kernel;
}

// calls processing function
void InterestPoint::run(Mat& in, vector<KeyPoint>& points) {
	this->getInterestPoints(in, this->sigma, points);
}

// non-maxima suppression
// Author: DR Ronny Hansh TU Berlin Computer Vision 
// Digital image processing Exercise5 
Mat InterestPoint::nonMaxSuppression(Mat& img) {

	Mat out = img.clone();

	for (int x = 1; x<out.cols - 1; x++) {
		for (int y = 1; y<out.rows - 1; y++) {
			if (img.at<float>(y - 1, x) >= img.at<float>(y, x)) {
				out.at<float>(y, x) = 0;
				continue;
			}
			if (img.at<float>(y, x - 1) >= img.at<float>(y, x)) {
				out.at<float>(y, x) = 0;
				continue;
			}
			if (img.at<float>(y, x + 1) >= img.at<float>(y, x)) {
				out.at<float>(y, x) = 0;
				continue;
			}
			if (img.at<float>(y + 1, x) >= img.at<float>(y, x)) {
				out.at<float>(y, x) = 0;
				continue;
			}
		}
	}
	return out;
}

// Function displays image 
// Author: DR Ronny Hansh TU Berlin Computer Vision 
// Digital image processing Exercise5 
void InterestPoint::showImage(Mat& img, const char* win, int wait, bool show, bool save) {

	Mat aux = img.clone();

	// scale and convert image
	if (img.channels() == 1)
		normalize(aux, aux, 0, 255, CV_MINMAX);
	aux.convertTo(aux, CV_8UC1);
	// show image
	if (show) {
		imshow(win, aux);
		waitKey(wait);
	}
	// save image
	if (save)
		imwrite((string(win) + string(".png")).c_str(), aux);
}