#pragma once
#include <vector>
#include <string>

const std::string PNG_EXT = ".png";
using uchar = unsigned char;
using KERNEL = std::vector<std::vector<int> >;

inline bool inBounds(int x, int y, int n, int m) {
	return (0 <= x && x < n && 0 <= y && y < m);
}