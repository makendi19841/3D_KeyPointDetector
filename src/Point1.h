#pragma once

/* To read the scan without attached color information 
If the scan has no colors attached to the points then each line is of the form:
x y z i
where (x,y,z) is the points 3D coordinates expressed in meters and i is the intensity
of the returned laser beam (0 < i < 1).
*/

#include<string>
#include<vector>
#include<iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Point1 {
public:

	float x, y, z, intensity;

	Point1::Point1(float my_x, float my_y, float my_z, float my_i);
	Point1::Point1();
	Point1::~Point1();
};



Point1::Point1(float my_x, float my_y, float my_z, float my_i) : x(my_x), y(my_y), z(my_z), intensity(my_i) {}
Point1::Point1() {}
Point1:: ~Point1() {}
