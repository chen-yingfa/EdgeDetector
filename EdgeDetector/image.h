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
	Image(uchar* data, int h, int w);
	~Image();

	uchar* at(const int x) const;
	uchar* at(const int x, const int y) const;

	void load(std::string filename);
	void save(std::string filename);

	Vec3 getColor(int x, int y) const;

	Image* subImage(int x, int y, int h, int w);
};


