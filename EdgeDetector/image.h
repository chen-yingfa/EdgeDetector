#pragma once
#include <string>
#include <vector>
#include "globals.h"
#include "Vec.h"


class Image
{
public:
	std::string filename;
	unsigned char* data;
	int width, height;
	int bytesPerRow;
	int bytesPerPixel = 3;
	int comps = 3;

	Image();
	Image(std::string filename);
	Image(uchar* data, int h, int w, std::string filename);
	~Image();

	uchar* at(const int x) const;
	uchar* at(const int x, const int y) const;

	void load(std::string filename);
	void save(std::string filename);

	Vec3 getColor(int x, int y) const;

	Image* subImage(int x, int y, int h, int w);
};

class BWImage {
public:
	float* data;
	int height, width;
	std::string filename;

	BWImage() : height(0), width(0), filename(""), data(nullptr) {}
	BWImage(Image* img);
	BWImage(int height, int width) 
		:	height(height),
			width(width),
			filename("") {
		data = new float[width * height];
	}
	BWImage(float* data, int h, int w, std::string filename);
	~BWImage();

	float at(const int x, const int y) const;
	

	BWImage* subImage(int x, int y, int h, int w);
};

Image* bwToColor(BWImage* bwImage);