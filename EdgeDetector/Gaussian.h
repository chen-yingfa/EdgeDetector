#pragma once
#include <thread>
#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include "image.h"
#include "Vec.h"

class Gaussian
{
public:
	float kernelSum_5 = 273.0;
	float kernelSum_7 = 1003.0;
	// float kernelSum = 25.0;
	
	KERNEL kernel_5 = { 
		{1,  4,  7,  4, 1},
		{4, 16, 26, 16, 4},
		{7, 26, 41, 26, 7},
		{4, 16, 26, 16, 4},
		{1,  4,  7,  4, 1}};

	KERNEL kernel_7 = {
		{0, 0, 1, 2, 1, 0, 0},
		{0, 3, 13, 22, 13, 3, 0},
		{1, 13, 59, 97, 59, 13, 1},
		{2, 22, 97, 159, 97, 22, 2},
		{1, 13, 59, 97, 59, 13, 1},
		{0, 3, 13, 22, 13, 3, 0},
		{0, 0, 1, 2, 1, 0, 0} };

	/*
	int kernel[KERNEL_SIZE][KERNEL_SIZE] = {
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1} };
	*/

	BWImage* image;
	float* resultData;
	int nThreads;

	Gaussian(int nThreads) : image(nullptr), resultData(nullptr), nThreads(nThreads) {
		assert(nThreads > 0);
	}
	~Gaussian() {
		
	}

	void setImage(BWImage* image) {
		this->image = image;
	}

	// void job(int xlo, int xhi);

	BWImage* execute();

	BWImage* process(BWImage* input);
};

