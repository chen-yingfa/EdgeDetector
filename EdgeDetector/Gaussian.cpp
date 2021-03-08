#include "Gaussian.h"
#include <iostream>

bool inline inBounds(int x, int y, int n, int m) {
	return (0 <= x && x < n && 0 <= y && y < m);
}

// NOTE: height of image must be >= xhi + KERNEL_SIZE
void job(Gaussian* gauss, int xlo, int xhi) {
	int w = gauss->image->width;
	int h = gauss->image->height;
	int ks = gauss->KERNEL_SIZE;
	int padding = ks / 2;
	for (int x = xlo; x < xhi; ++x) {
		for (int y = 0; y < w; ++y) {
			float val = 0;
			for (int i = 0; i < ks; ++i) {
				for (int j = 0; j < ks; ++j) {
					int row = x + i - padding;
					int col = y + j - padding;
					float pixel = 0.5;
					if (inBounds(row, col, h, w)) {
						pixel = gauss->image->at(row, col);
					}
					val += pixel * gauss->kernel[i][j] / gauss->kernelSum;
				}
			}
			float* dst = gauss->resultData + x * w + y;
			*dst += val;
		}
	}
}

BWImage* Gaussian::execute() {
	int w = image->width;
	int h = image->height;
	resultData = new float[h * w];
	for (int i = 0; i < h * w; ++i) {
		resultData[i] = 0.0;
	}
	int threadHeight = (h - KERNEL_SIZE) / nThreads;

	std::vector<std::thread> threads;

	for (int i = 0; i < nThreads; ++i) {
		int xlo = i * threadHeight;
		int xhi = std::min(xlo + threadHeight, h);
		threads.emplace_back(job, this, xlo, xhi);
	}

	for (auto& th : threads) {
		th.join();
	}
	
	return new BWImage(resultData, h, w, image->filename);
}