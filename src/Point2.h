#pragma once

/* To read the scan with attached color information
If the scan has colors attached to the points then each point is of the form:
x y z i r g b
x y z and i are as before and r g b are the red, green, and blue color components (0 <= r,g,b <= 255).
The r,g,b values have been acquired by color camera attached on the scanner.
*/

#include<string>
#include<vector>
#include<iostream>

using namespace std;

class Point2 {
public:
	int              r, g, b;
	float x, y, z, intensity;

	Point2::Point2(float my_x, float my_y, float my_z, float my_i, int my_r, int my_g, int my_b);
	Point2::Point2();
	Point2::~Point2();
};

Point2::Point2(float my_x, float my_y, float my_z, float my_i, int my_r, int my_g, int my_b) : x(my_x), y(my_y), z(my_z), intensity(my_i), r(my_r), g(my_g), b(my_b){}
Point2::Point2() {}
Point2::~Point2() {}
