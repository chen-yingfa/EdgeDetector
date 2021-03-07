#pragma once
#include <thread>
#include "image.h"
#include "Vec.h"

class Gaussian
{
public:
	static const int K_SIZE = 5;
	int kernel[K_SIZE][K_SIZE] = { 
		{1,  4,  7,  4, 1},
		{4, 16, 26, 16, 4},
		{7, 26, 41, 26, 7},
		{4, 16, 26, 16, 4},
		{1,  4,  7,  4, 1}};
	BWImage* image;
	BWImage* result;

	Gaussian() : image(nullptr), result(nullptr) {}

	void addImage(BWImage* image) {
		this->image = image;
	}

	void job(int xlo, int xhi) {
		for (int x = xlo; x < xhi; ++x) {
			for (int y = 0; y < image->width; ++y) {
				for (int i = 0; i < K_SIZE; ++i) {
					for (int j = 0; j < K_SIZE; ++j) {
						
					}
				}
			}
		}
	}

	BWImage* execute() {
		BWImage* result;

	}
};

