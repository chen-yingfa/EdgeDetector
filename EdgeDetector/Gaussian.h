#pragma once
#include <thread>
#include "image.h"

class Gaussian
{
public:
	int kernel[5][5] = { {1,  4,  7,  4, 1},
						{4, 16, 26, 16, 4},
						{7, 26, 41, 26, 7},
						{4, 16, 26, 16, 4},
						{1,  4,  7,  4, 1} };
	BWImage* image;
	BWImage* result;

	Gaussian() : image(nullptr), result(nullptr) {}

	void addImage(BWImage* image) {
		this->image = image;
	}

	void job(int xlo, int xhi) {
		for (int x = xlo; x < xhi; ++x) {
			for (int y = 0; y < image->width; ++y) {
				
			}
		}
	}

	BWImage* execute() {
		BWImage* result;
	}
};

