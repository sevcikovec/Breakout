#pragma once
#include "Vec3.h"
#include "ostream"


struct Mat3 {
public:
	Mat3(const float& i);
	Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2);
	Mat3(const Mat3& other);
	Mat3(
		const float& x0, const float& y0, const float& z0,
		const float& x1, const float& y1, const float& z1,
		const float& x2, const float& y2, const float& z2
	);
	Mat3 Transpose();
	Mat3 Inverse();
	float Det();
	Mat3 operator+(const Mat3& other);
	Mat3 operator-(const Mat3& other);
	Mat3 operator*(const Mat3& other);
	Mat3 operator*(const float& other);
	float operator[](int) const;

	float operator()(int row, int col) const;

	float& operator()(int row, int col);

	friend std::ostream& operator<<(std::ostream& os, const Mat3& v) {
		os << v[0] << ", " << v[3] << ", " << v[6] << std::endl;
		os << v[1] << ", " << v[4] << ", " << v[7] << std::endl;
		os << v[2] << ", " << v[5] << ", " << v[8] << std::endl;
		return os;
	}
private:
	Vec3 collumns[3];
};