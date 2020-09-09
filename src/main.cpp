
#include <cstdio>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
#include <iomanip>
#include"Point1.h"
#include"Point2.h"
#include "InterestPoint.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;

using namespace cv;



vector<Point1> ReadData1(string path) {

	string eee;
	vector<Point1> result;
	float x, y, z, intensity;

	ifstream file(path, ios::in);
	{
		getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee);
		getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee);
	}
	while (file && !file.eof())
	{
		file >> x >> y >> z >> intensity;
		result.push_back(Point1(x, y, z, intensity));
	}

	result.pop_back();

	return result;
}

vector<Point2> ReadData2(string path) {

	string eee;
	vector<Point2> result;
	float x, y, z, intensity,r,g,b;                                      
	                            
	ifstream file(path, ios::in);                                  
	{
		getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee);
		getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee); getline(file, eee);
	}
	while (file && !file.eof())                                    
	{
		file >> x >> y >> z >> intensity>>r>>g>>b;                      
		result.push_back(Point2(x, y, z, intensity,r,g,b));              
	}
	
	result.pop_back();
	
	return result;
}

void SaveHistogramResult(string histogram, int Histogram[]) {
	// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = histogram[0];
	for (int i = 1; i < 256; i++) {
		if (max < histogram[i]) {
			max = histogram[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < 255; i++) {
		histogram[i] = ((double)histogram[i] / max)*histImage.rows;
	}

	// draw the intensity line for histogram
	for (int i = 0; i < 255; i++)
	{
		line(histImage, Point(bin_w*(i), hist_h),
			Point(bin_w*(i), hist_h - histogram[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}

	// save histogram result
	string HistName = histogram + "IntensityHistogram";
	imwrite(HistName, histImage);
}

int main(int argc, char** argv) {
   

	cout << "//=====================================================================================================" << endl;
	cout << "// Name        : main.cpp" << endl;
	cout << "// Author      : Amos Makendi" << endl;
	cout << "// Version     : 1.0 " << endl;
	cout << "// Create on   : 07/02/2018" << endl;
	cout << "//" << endl;
	cout << "// Course      : 3D Laser Scanning" << endl;
	cout << "// Description : PTX import and generation images " << endl;
	cout << "//               Histogram stretching " << endl;
	cout << "//               Keypoint generation" << endl;
	cout << "//               Image registration" << endl;
	cout << "//" << endl;
	cout << "//               Image registration" << endl;
	cout << "// Topic       : Apply image processing techniques to Terrestrial Laser Scanning Data" << endl;
	cout << "//=======================================================================================================" << endl << endl << endl << endl << endl;



	cout << "//********************************* GrayScale Image Generation ****************************************" << endl;
	
	int value  = 960;
	int value2 = 1616;
	cout << "// Extract columns and rows grayscale image" << endl;
	cout << "Out1     :  number of rows: " << value << endl;
	cout << "Out2     :  number of cols: " << value2 << endl << endl << endl;

	std::vector<Point2> pts = ReadData2("Orangerie.ptx");           
	Mat img = Mat(960, 1616, CV_32FC1, Scalar(0));

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {

			img.at<float>(y, x) = pts[img.rows*x + y].intensity * 255;
		
		}
	}

	imwrite("GrayscaleImage.png", img);
	cout << " >> Done...." << endl << endl << endl;

	cout << "//********************************* Color Image Generation ********************************************" << endl;

	int value3 = 1145;
	int value4 = 684;
	cout << "// Extract columns and rows information color image" << endl;
	cout << "Out1     :  number of rows: " << value3 << endl;
	cout << "Out2     :  number of cols: " << value4 << endl << endl << endl;

	Vec3f pixel;
	std::vector<Point2> pts2 = ReadData2("TU-Main-Building.ptx");
	Mat img2 = Mat(1145, 684, CV_32FC3); // to change to show color image

	for (int y = 0; y < img2.rows; y++) {
		for (int x = 0; x < img2.cols; x++) {
			
		    pixel.val[0] = pts2[img2.rows*x + y].r;
			pixel.val[1] = pts2[img2.rows*x + y].b;
			pixel.val[2] = pts2[img2.rows*x + y].g;
		    img2.at <Vec3f>(y, x) = pixel ;

		}
	}
	imwrite("ColorImage.png", img2);
	cout << " >> Done...." << endl << endl << endl;




cout << "//**********************************Histogram Stretching***********************************************" << endl;
/*
step1: define range of pixel: [gmin , gmax]
step2: count number of each pixel in the defined range: [g(0),......,g(n)];
step3: find min and max pixel in the defined range
step4: normalize pixel by g = (f - min) / (max - min)
step5: visualize histogram
*/

Mat imgg = img.clone();

// pixel intensity memory allocation
int imgghistogram[256];

// pixel intensity initialization
for ( int i = 0; i < 256; i++) {
	imgghistogram[i] = 0;
}

// number of each intensity pixel  
for ( int y = 0; y < imgg.rows; y++) {
	for ( int x = 0; x < imgg.cols; x++) {   
		imgghistogram[(int)imgg.at<float>(y, x)]++;
	}
}


double gmin;
double gmax;
minMaxIdx(imgg, &gmin, &gmax);                                                    // determine the min and max grayscale image values

double _cpd;
int newimgghistogram[256];
for (int i = 0; i < 256; i++) {
	_cpd = ((double)imgghistogram[i] - gmin) / (gmax - gmin);                    // normalization of pixel: g(f) = ((pixel - gmin) / (gmax - gmin)) * 255
	newimgghistogram[i] = cvRound((int)((_cpd * 255)));
}


Mat resultImgg; 
for (int y = 0; y < resultImgg.rows; y++) {
	for (int x = 0; x < resultImgg.cols; x++) {
		resultImgg.at<float>(y, x) = newimgghistogram[(int)imgg.at<float>(y, y)];  // replace old pixel by new stretched pixel
	}   
}

// histogram after stretching       
int resultImgssHistogram[256];
int noPixelNewColorLevel;

for (int i = 0; i < 256; i++) {
	int noPixelNewColorLevel = 0;
	for (int j = 0; j < 256; j++) {
		if (i == newimgghistogram[j])
			noPixelNewColorLevel += imgghistogram[j];
	}
	resultImgssHistogram[i] = noPixelNewColorLevel;
}

// draw the histograms
int hist_w = 512; int hist_h = 400;
int bin_w = cvRound((double)hist_w / 256);

Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
Mat histImage2(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

// find the maximum intensity element from histogram
int max = newimgghistogram[0];
for (int i = 1; i < 256; i++) {
	if (max < newimgghistogram[i]) {
		max = newimgghistogram[i];
	}
}

int max1 = imgghistogram[0];
for (int i = 1; i < 256; i++) {
	if (max1 < imgghistogram[i]) {
		max1 = imgghistogram[i];
	}
}

// normalize the histogram between 0 and histImage.rows
for (int i = 0; i < 255; i++) {
	newimgghistogram[i] = ((double)newimgghistogram[i] / max1)*histImage2.rows;
	imgghistogram[i] = ((double)imgghistogram[i] / max1)*histImage2.rows;
}


// draw the intensity line for histogram
for (int i = 0; i < 255; i++)
{
	line(histImage, Point(bin_w*(i), hist_h),
		Point(bin_w*(i), hist_h - newimgghistogram[i]),
		Scalar(0, 0, 0), 1, 8, 0);
}

for (int i = 0; i < 255; i++)
{
	line(histImage2, Point(bin_w*(i), hist_h),
		Point(bin_w*(i), hist_h - imgghistogram[i]),
		Scalar(0, 0, 0), 1, 8, 0);
}


// save histogram results
imwrite("HistogramStretched_Orangerie.jpg", histImage);
imwrite("HistogramOriginal_Orangerie.jpg", histImage2);
cout << " >> Done...." << endl << endl << endl;

cout << "//********************************* KeyPoint Generation: Försstner*************************************" << endl;
/*
    step1: compute gradient Gx = img * gx and Gy = img * gy
	step2: compute GxGx, GyGy, GxGy
	step3: blur GxGx, GyGy, GxGy using Gaussian
	step4: compute structure tensor: A
	step5: compute trace(A) and det(A)
	step6: estimate point likeness(w) and ellipse roundness(q)
	step7: non maximum suppression operation
	step7: check if point or blob exist
*/

// check if enough arguments are defined
if (argc < 1) {
	cout << "ERROR: sigma not specified" << endl;
	cout << "Press enter to exit" << endl;
	cin.get();
	return -1;
}

Mat img1 = img.clone();

// define standard deviation of directional gradient
double sigma;
if (argc < 3)
	sigma = 0.5;
else
sigma = atof(argv[1]);

// construct processing object
InterestPoint interestpts (sigma);

// calculate interest points grayscale image
vector<KeyPoint> points;
interestpts.run(img1, points);

// calculate interest points grayscale image
vector<KeyPoint> points2;
interestpts.run(img2, points2);

// Save keypoints for grayscale 
vector<Point2> keypoints_txt;
for (int i = 0; i < points.size(); i++) {
	keypoints_txt.push_back(pts[points[i].pt.x*img1.rows + points[i].pt.y]);
}


// Save keypoints for color image
vector<Point2> keypoints_txt2;
for (int i = 0; i < points2.size(); i++) {
	keypoints_txt2.push_back(pts2[points2[i].pt.x*img2.rows + points2[i].pt.y]);
}


// Export 3D-points from greyscale and color images into a txt file
ofstream opfile;
opfile.open("keyPoint_Orangerie.txt");
for (int i = 0; i < keypoints_txt.size(); i++) {
	opfile << keypoints_txt[i].x << " " << keypoints_txt[i].y << " " << keypoints_txt[i].z<< " " << keypoints_txt[i].intensity<< " " << keypoints_txt[i].r<< " " << keypoints_txt[i].g<< " " << keypoints_txt[i].b<< endl;
}
opfile.close();

ofstream opfile2;
opfile2.open("keyPoint_TU-Main-Building.txt");
for (int i = 0; i < keypoints_txt2.size(); i++) {
	opfile2 << keypoints_txt2[i].x << " " << keypoints_txt2[i].y << " " << keypoints_txt2[i].z << " " << keypoints_txt2[i].intensity << " " << keypoints_txt2[i].r << " " << keypoints_txt2[i].g << " " << keypoints_txt2[i].b << endl;
}
opfile2.close();

cout << "Number of Keypoints Orangerie:"<< keypoints_txt.size() << endl;
cout << "Number of Keypoints TU-Main-Building:"<< keypoints_txt2.size() << endl << endl << endl;
cout << " >> Done...." << endl << endl << endl;

cout << "//********************************* Plot keypoint on grayscale image*************************************" << endl;
drawKeypoints(img1, points, img1, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG + DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
interestpts.showImage(img1, "keypoints_Orangerie", 0, true, true);
cout << " >> Done...." << endl << endl << endl;


cout << "//********************************* Plot keypoint on color image********************************************" << endl;
drawKeypoints(img2, points2, img2, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG + DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
interestpts.showImage(img2, "keypoints_TU-Main-Building", 0, true, true);
cout << " >> Done...." << endl << endl << endl;

return 0;
system("pause");
}

