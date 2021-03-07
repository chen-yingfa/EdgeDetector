#pragma once
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
#include "image.h"
#include "Vec.h"

class Gaussian
{
public:
	static const int KERNEL_SIZE = 5;
	// float kernelSum = 273.0;
	float kernelSum = 25.0;
	/*
	int kernel[KERNEL_SIZE][KERNEL_SIZE] = { 
		{1,  4,  7,  4, 1},
		{4, 16, 26, 16, 4},
		{7, 26, 41, 26, 7},
		{4, 16, 26, 16, 4},
		{1,  4,  7,  4, 1}};
		*/
	int kernel[KERNEL_SIZE][KERNEL_SIZE] = {
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1} };
	BWImage* image;
	float* resultData;
	int nThreads;

	Gaussian(int nThreads) : image(nullptr), resultData(nullptr), nThreads(nThreads) {}
	~Gaussian() {
		
	}

	void setImage(BWImage* image) {
		this->image = image;
	}

	// void job(int xlo, int xhi);

	BWImage* execute();
};

