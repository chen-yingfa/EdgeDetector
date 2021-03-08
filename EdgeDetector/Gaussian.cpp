#include "Gaussian.h"
#include <iostream>

// kernel MUST NOT be empty.
void conv(BWImage* image, const KERNEL kernel, BWImage* result, int xlo, int xhi) {
	// std::cout << "conv " << xlo << " " << xhi << "\n";
	int w = image->width;
	int h = image->height;
	int kh = kernel.size();
	int kw = kernel[0].size();
	int ks = kh * kw;
	int paddingH = kh / 2;
	int paddingW = kw / 2;

	float kernelSum = 0.0;
	for (auto& v : kernel) {
		for (int a : v) {
			kernelSum += (float) a;
		}
	}
	// std::cout << "kernel sum: " << kernelSum << "\n";

	for (int x = xlo; x < xhi; ++x) {
		for (int y = 0; y < w; ++y) {
			float val = 0;
			for (int i = 0; i < kh; ++i) {
				for (int j = 0; j < kw; ++j) {
					int row = x + i - paddingH;
					int col = y + j - paddingW;
					float pixel = 0.5;
					if (inBounds(row, col, h, w)) {
						pixel = image->at(row, col);
					}
					val += pixel * kernel[i][j];
				}
			}
			val /= kernelSum;
			result->set(x, y, val);
		}
	}
}

BWImage* Gaussian::execute() {
	int w = image->width;
	int h = image->height;
	BWImage* res = new BWImage(h, w);

	// choose kernel according to size of the input image
	KERNEL* ker = &kernel_5;
	if (h > 1600 || w > 1600) {
		ker = &kernel_7;
	}

	// init threads
	std::vector<std::thread> threads;
	int threadHeight = (h - 1) / nThreads + 1;
	// std::cout << "thread height: " << threadHeight << "\n";

	for (int i = 0; i < nThreads; ++i) {
		int xlo = i * threadHeight;
		int xhi = std::min(xlo + threadHeight, h);
		// std::cout << xlo << " " << xhi << "\n";
		threads.emplace_back(conv, image, *ker, res, xlo, xhi);
	}
	// execute
	for (auto& th : threads) {
		th.join();
	}
	
	return res;
}

BWImage* Gaussian::process(BWImage* input) {
	setImage(input);
	return execute();
}