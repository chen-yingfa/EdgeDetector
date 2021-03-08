#include "Canny.h"
#include "image.h"
#include <iostream>
using namespace std;


BWImage* convolve(BWImage* img, vector<vector<int>> kernel) {
	int h = img->height;
	int w = img->width;
	int kh = kernel.size();
	int kw = kernel[0].size();
	float ks = kh * kw;
	int padding = kh / 2; // boundary pixels of the image


	float* res = new float[h * w];
	for (int i = 0; i < h * w; ++i) {
		res[i] = 0;
	}
	for (int y = padding; y < h - padding; ++y) {
		for (int x = padding; x < w - padding; ++x) {
			float intensity = 0;
			for (int ky = 0; ky < kh; ++ky) {
				for (int kx = 0; kx < kw; ++kx) {
					int row = y + ky - padding;
					int col = x + kx - padding;
					float p = img->at(0, 0);
					float r = img->at(row, col);
					intensity += img->at(row, col) * kernel[ky][kx];
				}
			}
			float* dst = res + y * w + x;
			*dst = intensity / ks;
		}
	}
	return new BWImage(res, h, w, "a.png");
}



BWImage* magnitude(BWImage* gx, BWImage* gy) {
	int w = gx->width;
	int h = gx->height;
	float* magnitude = new float[w * h];
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			magnitude[y * w + x] = hypot(gx->at(y, x), gy->at(y, x));
		}
	}
	return new BWImage(magnitude, h, w, "");
}

// Make the image more visible by scaling the intensities
void normalize(BWImage* img) {
	int w = img->width;
	int h = img->height;
	float max = 0.0;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			float intensity = img->at(y, x);
			intensity = std::max(0.0f, intensity);
			max = std::max(max, intensity);
		}
	}
	for (int i = 0; i < w * h; ++i) {
		*(img->data + i) = *(img->data + i) / max;
	}
}




BWImage* Canny::execute() {
	int w = image->width;
	int h = image->height;
	resultData = new float[h * w];
	for (int i = 0; i < h * w; ++i) {
		resultData[i] = 0.0;
	}


	// Find gradients
	vector<vector<int>> sobel_x{ {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1 } };
	vector<vector<int>> sobel_y{ {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	BWImage* gx = convolve(image, sobel_x);
	BWImage* gy = convolve(image, sobel_y);

	// Print save sobel x and y
	Image* tmpImg = bwToColor(gx);
	tmpImg->save("..\\input\\sobelx.test.png");
	tmpImg = bwToColor(gy);
	tmpImg->save("..\\input\\sobely.test.png");
	
	BWImage* gradientMagnitude = magnitude(gx, gy);
	normalize(gradientMagnitude);

	return gradientMagnitude;

	return new BWImage(resultData, h, w, image->filename);
}