#pragma once
#include <cmath>
#include <string>

class Vec3 {
private:
	int elems[3];

public:
	Vec3(int x, int y, int z);
	Vec3(const Vec3& other);

	static const Vec3 ZERO;
	static const Vec3 FULL;
	static const Vec3 RED;
	static const Vec3 GREEN;
	static const Vec3 BLUE;

	float x() const;
	float y() const;
	float z() const;

	// utilities
	Vec3& operator=(const Vec3&);
	Vec3& operator+=(const Vec3&);
	Vec3& operator-=(const Vec3&);
	Vec3& operator*=(const Vec3&);
	Vec3& operator/=(const Vec3&);
	Vec3& operator*=(float);
	Vec3& operator/=(float);

	const int& operator[](int i) const;
	int& operator [](int i);

	static float dot(const Vec3&, const Vec3&);
	static Vec3 cross(const Vec3&, const Vec3&);
	static float angleBetween(const Vec3&, const Vec3&);

	float len() const;
	float lenSq() const;

	// debug utilities
	std::string str() const;
};

// component-wise operators
Vec3 operator + (const Vec3& v0, const Vec3& v1);
Vec3 operator - (const Vec3& v0, const Vec3& v1);
Vec3 operator * (const Vec3& v0, const Vec3& v1);
Vec3 operator / (const Vec3& v0, const Vec3& v1);

// unary negation
Vec3 operator - (const Vec3& v);

// multiply and divide by scalar
Vec3 operator * (float f, const Vec3& v);
Vec3 operator * (const Vec3& v, float f);
Vec3 operator / (const Vec3& v, float f);

bool operator == (const Vec3& v0, const Vec3& v1);
bool operator != (const Vec3& v0, const Vec3& v1);