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

	int size() const { return height * width; }

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
	BWImage(int height, int width);
	BWImage(float* data, int h, int w, std::string filename);
	~BWImage();

	void save(std::string filename) const;

	int size() const { return height * width; }

	inline float at(const int x, const int y) const;
	void set(const int x, const int y, const float val);
	

	BWImage* subImage(int x, int y, int h, int w);
};

Image* bwToColor(BWImage* bwImage);