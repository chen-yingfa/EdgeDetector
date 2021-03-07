#include <cstdio>
#include <iostream>
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image() :
		filename(""),
		data(nullptr),
		width(0),
		height(0),
		bytesPerRow(0) {}

Image::Image(std::string filename, bool bw = false) :
		filename(filename) {
	data = stbi_load(filename.c_str(), &width, &height, &comps, bytesPerPixel);
	if (!data) {
		throw(std::string("Load image failed"));
	}
	this->bytesPerRow = bytesPerPixel * width;

	if (bw) { // turn in to black and white
		float* bwdata = new float[width * height];
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int r = *data;
				int g = data[1];
				int b = data[2];
				bwdata[i++] = 0.212 * r + 0.7152 * 
			}
		}
	}


}
Image::Image(uchar* data, int h, int w, bool bw = false) {
	if (bw) {
		bytesPerPixel = 1;
	}
	this->data = data;
	height = h;
	width = w;
	bytesPerRow = bytesPerPixel * width;
}

Image::~Image() {
	if (data != nullptr) {
		delete[] data;
	}
}

uchar* Image::at(const int x, const int y) const {
	return data + x * bytesPerRow + y * bytesPerPixel;
}

void Image::load(std::string filename) {
	data = stbi_load(filename.c_str(), &width, &height, &comps, bytesPerPixel);
	if (!data) {
		throw(std::string("Load image failed"));
	}
	bytesPerRow = bytesPerPixel * width;
}

void Image::save(std::string filename) {
	std::cout << "saving to " << filename << "\n";

	int succ = stbi_write_png(filename.c_str(), width, height, bytesPerPixel, data, width * bytesPerPixel);
	if (!succ) {
		throw(std::string("Save image failed"));
	}
}

Vec3 Image::getColor(int x, int y) const {
	uchar* pixel = at(x, y);
	printf("pixel = %p\n", pixel);
	return Vec3(pixel[0], pixel[1], pixel[2]);
}



Image* Image::subImage(int x, int y, int h, int w) {
	// std::cout << "h, w = " << h << ", " << w << "\n";
	// std::cout << "num pixels: " << h * w << "\n";
	uchar* newData = new uchar[h * w * bytesPerPixel];
	uchar* p = newData;
	int newBytesPerRow = w * bytesPerPixel;

	
	for (int i = 0; i < h; ++i) {
		uchar* src = at(x + i, y);
		uchar* dst = newData + i * newBytesPerRow;
		//printf("%p %p\n", src, dst);
		memcpy(dst, src, newBytesPerRow);
	}
	

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			uchar* src = at(x + i, y + j);
			uchar* dst = newData + i * newBytesPerRow + j * bytesPerPixel;
			//memcpy(dst, src, bytesPerPixel);
		}
	}
	return new Image(newData, h, w);
}



BWImage::BWImage(Image* img) {
	width = img->width;
	height = img->height;
	filename = img->filename;
	data = new float[width * height];

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			uchar* pixel = img->at(i, j);
			float r = (float) pixel[0];
			float g = (float) pixel[1];
			float b = (float) pixel[2];
			*(data + i * width + j) = 0.212 * r + 0.7152 * g + 0.0722 * b;
		}
	}
}

BWImage::~BWImage() {
	delete[] data;
}

float BWImage::at(const int x, const int y) const {
	return *(data + width * x + y);
}

BWImage* BWImage::subImage(int x, int y, int h, int w) {
	
}