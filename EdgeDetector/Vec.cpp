#include "Vec.h"


const Vec3 Vec3::ZERO = Vec3(0, 0, 0);
const Vec3 Vec3::FULL = Vec3(255, 255, 255);
const Vec3 Vec3::RED = Vec3(255, 0, 0);
const Vec3 Vec3::GREEN = Vec3(0, 255, 0);
const Vec3 Vec3::BLUE = Vec3(0, 0, 255);

Vec3::Vec3(int x, int y, int z) {
	elems[0] = x;
	elems[1] = y;
	elems[2] = z;
}

Vec3::Vec3(const Vec3& other) {
	for (int i = 0; i < 2; ++i) {
		elems[i] = other[i];
	}
}

float Vec3::x() const {
	return elems[0];
}

float Vec3::y() const {
	return elems[1];
}

float Vec3::z() const {
	return elems[2];
}


int& Vec3::operator[](int i) {
	return elems[i];
}

const int& Vec3::operator[](int i) const {
	return elems[i];
}


Vec3& Vec3::operator=(const Vec3& other) {
	if (this != &other) {
		for (int i = 0; i < 3; ++i) elems[i] = other[i];
	}
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& other) {
	for (int i = 0; i < 3; ++i) elems[i] += other[i];
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& other) {
	for (int i = 0; i < 3; ++i) elems[i] -= other[i];
	return *this;
}
Vec3& Vec3::operator*=(const Vec3& other) {
	for (int i = 0; i < 3; ++i) elems[i] *= other[i];
	return *this;
}
Vec3& Vec3::operator/=(const Vec3& other) {
	for (int i = 0; i < 3; ++i) elems[i] /= other[i];
	return *this;
}
Vec3& Vec3::operator*=(float f) {
	for (int i = 0; i < 3; ++i) elems[i] *= f;
	return *this;
}
Vec3& Vec3::operator/=(float f) {
	for (int i = 0; i < 3; ++i) elems[i] /= f;
	return *this;
}

inline float Vec3::lenSq() const {
	return dot(*this, *this);
}

inline float Vec3::len() const {
	return sqrt(lenSq());
}

// static
inline float Vec3::dot(const Vec3& a, const Vec3& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// static
inline Vec3 Vec3::cross(const Vec3& v0, const Vec3& v1) {
	return Vec3(
		v0.y() * v1.z() - v0.z() * v1.y(),
		v0.z() * v1.x() - v0.x() * v1.z(),
		v0.x() * v1.y() - v0.y() * v1.x()
	);
}

// static
float Vec3::angleBetween(const Vec3& v0, const Vec3& v1) {
	return acos(dot(v0, v1) / (v0.len() * v1.len()));
}


Vec3 operator + (const Vec3& v0, const Vec3& v1)
{
	return Vec3(
		v0.x() + v1.x(), 
		v0.y() + v1.y(), 
		v0.z() + v1.z()
	);
}

Vec3 operator - (const Vec3& v0, const Vec3& v1)
{
	return Vec3(
		v0.x() - v1.x(), 
		v0.y() - v1.y(), 
		v0.z() - v1.z()
	);
}

Vec3 operator * (const Vec3& v0, const Vec3& v1)
{
	return Vec3(
		v0.x() * v1.x(), 
		v0.y() * v1.y(), 
		v0.z() * v1.z()
	);
}

Vec3 operator / (const Vec3& v0, const Vec3& v1)
{
	return Vec3(
		v0.x() / v1.x(), 
		v0.y() / v1.y(), 
		v0.z() / v1.z()
	);
}

Vec3 operator - (const Vec3& v)
{
	return Vec3(-v[0], -v[1], -v[2]);
}

Vec3 operator * (float f, const Vec3& v)
{
	return Vec3(
		v[0] * f, 
		v[1] * f, 
		v[2] * f
	);
}

Vec3 operator * (const Vec3& v, float f)
{
	return Vec3(
		v[0] * f, 
		v[1] * f, 
		v[2] * f
	);
}

Vec3 operator / (const Vec3& v, float f)
{
	return Vec3(
		v[0] / f, 
		v[1] / f, 
		v[2] / f
	);
}

bool operator == (const Vec3& v0, const Vec3& v1)
{
	return(v0.x() == v1.x() && v0.y() == v1.y() && v0.z() == v1.z());
}

bool operator != (const Vec3& v0, const Vec3& v1)
{
	return !(v0 == v1);
}

std::string Vec3::str() const {
	return "<" + std::to_string(elems[0])
		+ ", " + std::to_string(elems[1])
		+ ", " + std::to_string(elems[2])
		+ ">";
}
