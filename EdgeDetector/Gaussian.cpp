#include "Gaussian.h"
#include <iostream>

// NOTE: height of image must be >= xhi + KERNEL_SIZE
void job(Gaussian* gauss, int xlo, int xhi) {
	int w = gauss->image->width;
	for (int x = xlo; x < xhi; ++x) {
		for (int y = 0; y < w - gauss->KERNEL_SIZE; ++y) {
			for (int i = 0; i < gauss->KERNEL_SIZE; ++i) {
				for (int j = 0; j < gauss->KERNEL_SIZE; ++j) {
					int row = x + i;
					int col = y + j;
					float val = gauss->image->at(row, col) * gauss->kernel[i][j] / gauss->kernelSum;
					float* dst = gauss->resultData + row * w + col;
					// std::cout << row << " " << col << "\n";
					*dst = (*dst) + val;
				}
			}
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
		std::thread thr(job, this, xlo, xhi);
		threads.push_back(move(thr));
	}

	for (auto& th : threads) {
		th.join();
	}
	
	return new BWImage(resultData, h, w, image->filename);
}