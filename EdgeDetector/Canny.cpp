#include "Canny.h"
#include "image.h"
#include <iostream>
using namespace std;

# define PI	3.14159265358979323

float bound(float f, float min, float max) {
	if (f < min) return min;
	if (f > max) return max;
	return f;
}

void convolve(BWImage* img, vector<vector<int>> kernel, BWImage* result, int xlo, int xhi) {
	int h = img->height;
	int w = img->width;
	int kh = kernel.size();
	int kw = kernel[0].size();
	float ks = kh * kw;
	int paddingH = kh / 2; // boundary pixels of the image
	int paddingW = kw / 2;

	for (int x = xlo; x < xhi; ++x) {
		for (int y = 0; y < w - kw; ++y) {
			// sum of element-wise product
			float intensity = 0;
			for (int kx = 0; kx < kh; ++kx) {
				for (int ky = 0; ky < kw; ++ky) {
					int row = x + kx - paddingH;
					int col = y + ky - paddingW;
					float val = 0.5;
					if (inBounds(row, col, h, w)) {
						val = img->at(row, col);
					}
					intensity += val * kernel[kx][ky];
				}
			}
			result->set(x, y, intensity / ks);
		}
	}
}


// for multithreading
void magnitudeJob(BWImage* gx, BWImage* gy, BWImage* result, int xhi, int xlo) {
	int w = gx->width;
	int h = gx->height;
	for (int x = 0; x < h; ++x) {
		for (int y = 0; y < w; ++y) {
			float val = hypot(gx->at(x, y), gy->at(x, y));
			val = bound(val, 0.0f, 1.0f);
			result->set(x, y, val);
		}
	}
}

void magnitude(BWImage* gx, BWImage* gy, BWImage** result, int nThreads) {
	int w = gx->width;
	int h = gx->height;
	*result = new BWImage(h, w);

	std::vector<std::thread> threads;

	int threadHeight = h / nThreads;

	for (int i = 0; i < nThreads; ++i) {
		int xlo = i * threadHeight;
		int xhi = std::min(h, xlo + threadHeight);
		threads.emplace_back(magnitudeJob, gx, gy, *result, xhi, xlo);
	}

	for (auto& thr : threads) {
		thr.join();
	}
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
	*gx = new BWImage(h, w);
	*gy = new BWImage(h, w);

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

BWImage* hysteresis(BWImage* image, float tmin, float tmax) {
	static int adj[8][2] = {
		{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
		{0, 1}, {1, -1}, {1, 0}, {1, 1} };

	int h = image->height;
	int w = image->width;
	BWImage* res = new BWImage(h, w);	// should only consist of 1.0 and 0.0

	float* data = res->data;
	std::queue<pair<int, int>> q;
	for (int x = 1; x < h - 1; ++x) {
		for (int y = 1; y < w - 1; ++y) {
			if (image->at(x, y) < tmax || res->at(x, y) > 0.5) continue;
			
			// recursively look for semi-bright adjacent pixels
			res->set(x, y, 1.0);
			q.push({ x, y });
			while (!q.empty()) {
				int curx = q.front().first;
				int cury = q.front().second;
				q.pop();

				for (int i = 0; i < 8; ++i) {
					int nx = curx + adj[i][0];
					int ny = cury + adj[i][1];

					if (!inBounds(nx, ny, h, w)) continue;

					if (image->at(nx, ny) > tmin && res->at(nx, ny) < 0.5) {
						res->set(nx, ny, 1.0);
						q.push({ nx, ny });
					}
				}
			}
		}
	}
	return res;
}

BWImage* Canny::execute() {
	static int id = 0;
	int w = image->width;
	int h = image->height;
	int size = h * w;

	// Find gradients
	BWImage* gx = nullptr;
	BWImage* gy = nullptr;
	findGrad(image, &gx, &gy, nThreads);

	// Print save sobel x and y
	gx->save("..\\output\\" + std::to_string(id) + "sobelx.png");
	gy->save("..\\output\\" + std::to_string(id) + "sobely.png");

	BWImage* gradMagnitude = nullptr;
	magnitude(gx, gy, &gradMagnitude, nThreads);

	gradMagnitude->save("..\\output\\" + std::to_string(id) + "magned.png");

	nonMaximaSuppresion(gradMagnitude, gx, gy);
	normalize(gradMagnitude);

	gradMagnitude->save("..\\output\\" + std::to_string(id) + "normed.png");

	BWImage* result = hysteresis(gradMagnitude, tmin, tmax);

	++id;
	return result;
}

BWImage* Canny::process(BWImage* input) {
	setImage(input);
	return execute();
}