#pragma once
#include <thread>
#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include "image.h"
#include "Vec.h"
#include <queue>

class Canny
{
public:

	BWImage* image;
	float* resultData;
	int nThreads;
	float tmin = 0.06, tmax = 0.12;

	Canny(int nThreads) : 
		image(nullptr), 
		resultData(nullptr), 
		nThreads(nThreads) {
		assert(nThreads > 0);
	}
	Canny(int nThreads, int tmin, int tmax) 
		: image(nullptr), resultData(nullptr), 
		nThreads(nThreads), tmin(tmin), tmax(tmax) {}
	~Canny() {

	}

	void setImage(BWImage* image) {
		this->image = image;
	}

	// void job(int xlo, int xhi);
	BWImage* execute();
	BWImage* process(BWImage* input);
};

