#pragma once
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
#include "image.h"
#include "Vec.h"

class Canny
{
public:

	BWImage* image;
	float* resultData;
	int nThreads;

	Canny(int nThreads) : image(nullptr), resultData(nullptr), nThreads(nThreads) {}
	~Canny() {

	}

	void setImage(BWImage* image) {
		this->image = image;
	}

	// void job(int xlo, int xhi);

	BWImage* execute();
};

