#include "Canny.h"
#include "image.h"
#include <iostream>
using namespace std;

# define PI	3.14159265358979323

void convolve(BWImage* img, vector<vector<int>> kernel, BWImage* result, int xlo, int xhi) {
	int h = img->height;
	int w = img->width;
	int kh = kernel.size();
	int kw = kernel[0].size();
	float ks = kh * kw;
	int padding = kh / 2; // boundary pixels of the image

	for (int y = xlo; y < xhi; ++y) {
		for (int x = 0; x < w - kw; ++x) {
			float intensity = 0;
			for (int ky = 0; ky < kh; ++ky) {
				for (int kx = 0; kx < kw; ++kx) {
					int row = y + ky;
					int col = x + kx;
					float p = img->at(0, 0);
					float r = img->at(row, col);
					intensity += img->at(row, col) * kernel[ky][kx];
				}
			}
			int row = y + kh / 2;
			int col = x + kw / 2;;
			float* dst = result->data + row * w + col;
			*dst = intensity / ks;
		}
	}
}



// for multithreading
void magnitudeJob(BWImage* gx, BWImage* gy, BWImage* result, int xhi, int xlo) {
	int w = gx->width;
	int h = gx->height;
	float* data = result->data;
	for (int y = xlo; y < xhi; ++y) {
		for (int x = 0; x < w; ++x) {
			data[y * w + x] = hypot(gx->at(y, x), gy->at(y, x));
		}
	}
}


BWImage* magnitude(BWImage* gx, BWImage* gy, int nThreads) {
	int w = gx->width;
	int h = gx->height;
	float* magnitude = new float[w * h];
	BWImage* result = new BWImage(magnitude, h, w, "");

	std::vector<std::thread> threads;

	int threadHeight = h / nThreads;

	for (int i = 0; i < nThreads; ++i) {
		int xlo = i * threadHeight;
		int xhi = std::min(h, xlo + threadHeight);
		threads.emplace_back(magnitudeJob, gx, gy, result, xhi, xlo);
	}

	for (auto& thr : threads) {
		thr.join();
	}
	return result;
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

void findGrad(BWImage* image, BWImage** gx, BWImage** gy, int nThreads) {
	int w = image->width;
	int h = image->height;
	int size = h * w;

	vector<vector<int>> sobel_x{ {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1 } };
	vector<vector<int>> sobel_y{ {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	float* gxdata = new float[size];
	float* gydata = new float[size];
	for (int i = 0; i < size; ++i) {
		gxdata[i] = 0.0;
		gydata[i] = 0.0;
	}

	*gx = new BWImage(gxdata, h, w, "");
	*gy = new BWImage(gydata, h, w, "");

	std::vector<std::thread> threads;
	int threadHeight = h / nThreads;

	for (int i = 0; i < nThreads; ++i) {
		int xlo = i * threadHeight;
		int xhi = std::min(h, xlo + threadHeight);
		threads.emplace_back(convolve, image, sobel_x, *gx, xlo, xhi);
		threads.emplace_back(convolve, image, sobel_y, *gy, xlo, xhi);

	}

	for (auto& thr : threads) {
		thr.join();
	}
}


// For each pixel in the gradient magnitude image, make sure it is the local maximum 
// along the direction of the gradient
// This is called non-maxima suppresion
void nonMaximaSuppresion(BWImage* gradMagnitude, BWImage* gx , BWImage* gy) {
	int w = gradMagnitude->width;
	int h = gradMagnitude->height;
	for (int y = 1; y < h - 1; ++y) {
		for (int x = 1; x < w - 1; ++x) {
			float angle = atan2f(gy->at(y, x), gx->at(y, x));
			// 4 directions
			float dir = 8 * std::fmod(angle + PI, PI) / PI;
			float intensity = gradMagnitude->at(y, x);
			float neighbor[2] = { 0, 0 };
			if (dir > 1 && dir <= 3) {	// 0 deg
				neighbor[0] = gradMagnitude->at(y, x + 1);
				neighbor[1] = gradMagnitude->at(y, x - 1);
			}
			else if (dir > 3 && dir <= 5) {	// 45 deg
				neighbor[0] = gradMagnitude->at(y - 1, x + 1);
				neighbor[1] = gradMagnitude->at(y + 1, x - 1);
			}
			else if (dir > 5 && dir <= 7) {	// 90 deg
				neighbor[0] = gradMagnitude->at(y - 1, x);
				neighbor[1] = gradMagnitude->at(y + 1, x);
			}
			else if (dir > 7 || dir <= 1) {	// 90 deg
				neighbor[0] = gradMagnitude->at(y - 1, x - 1);
				neighbor[1] = gradMagnitude->at(y + 1, x + 1);
			}

			// If the magnitude of the gradient at the pixel is less than any of its neighbors along the direction (forward and backward)
			if (gradMagnitude->at(y, x) < neighbor[0] || gradMagnitude->at(y, x) < neighbor[1]) {
				gradMagnitude->data[y * w + x] = 0;
			}
		}
	}
}


BWImage* Canny::execute() {
	int w = image->width;
	int h = image->height;
	int size = h * w;

	// Find gradients
	BWImage* gx = nullptr, * gy = nullptr;
	findGrad(image, &gx, &gy, nThreads);

	// Print save sobel x and y
	Image* tmpImg = bwToColor(gx);
	tmpImg->save("..\\input\\sobelx.test.png");
	tmpImg = bwToColor(gy);
	tmpImg->save("..\\input\\sobely.test.png");
	
	BWImage* gradientMagnitude = magnitude(gx, gy, nThreads);

	nonMaximaSuppresion(gradientMagnitude, gx, gy);
	normalize(gradientMagnitude);

	return gradientMagnitude;

	return new BWImage(resultData, h, w, image->filename);
}