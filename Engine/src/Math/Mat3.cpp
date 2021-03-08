#include "Mat3.h"

namespace Engine
{
	Mat3::Mat3(const float& i)
	{
		this->collumns[0] = Vec3(i, 0, 0);
		this->collumns[1] = Vec3(0, i, 0);
		this->collumns[2] = Vec3(0, 0, i);
	}


	Mat3::Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2)
	{
		this->collumns[0] = v0;
		this->collumns[1] = v1;
		this->collumns[2] = v2;
	}

	Mat3::Mat3(const Mat3& other)
	{
		this->collumns[0] = other.collumns[0];
		this->collumns[1] = other.collumns[1];
		this->collumns[2] = other.collumns[2];
	}

	Mat3::Mat3(const float& x0, const float& y0, const float& z0, const float& x1, const float& y1, const float& z1, const float& x2, const float& y2, const float& z2)
	{
		this->collumns[0] = Vec3(x0, x1, x2);
		this->collumns[1] = Vec3(y0, y1, y2);
		this->collumns[2] = Vec3(z0, z1, z2);
	}

	Mat3 Mat3::Transpose()
	{
		return Mat3(
			(*this)[0], (*this)[1], (*this)[2],
			(*this)[3], (*this)[4], (*this)[5],
			(*this)[6], (*this)[7], (*this)[8]
		);
	}

	Mat3 Mat3::Inverse()
	{
		// https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c
		float d = Det();
		// NonInvertible matrix
		if (d == 0) d = 0.000001f;
		float invDet = 1 / d;

		return Mat3(
			((*this)[4] * (*this)[8] - (*this)[7] * (*this)[5]) * invDet, ((*this)[5] * (*this)[6] - (*this)[3] * (*this)[8]) * invDet, ((*this)[3] * (*this)[7] - (*this)[6] * (*this)[4]) * invDet,
			((*this)[2] * (*this)[7] - (*this)[1] * (*this)[8]) * invDet, ((*this)[0] * (*this)[8] - (*this)[2] * (*this)[6]) * invDet, ((*this)[6] * (*this)[1] - (*this)[0] * (*this)[7]) * invDet,
			((*this)[1] * (*this)[5] - (*this)[2] * (*this)[4]) * invDet, ((*this)[3] * (*this)[2] - (*this)[0] * (*this)[5]) * invDet, ((*this)[0] * (*this)[4] - (*this)[3] * (*this)[1]) * invDet
		);
	}

	float Mat3::Det()
	{
		return
			(*this)[0] * (*this)[4] * (*this)[8] +
			(*this)[3] * (*this)[7] * (*this)[2] +
			(*this)[6] * (*this)[1] * (*this)[5] -
			(*this)[6] * (*this)[4] * (*this)[2] -
			(*this)[3] * (*this)[1] * (*this)[8] -
			(*this)[0] * (*this)[7] * (*this)[5];
	}


	Mat3 Mat3::operator+(const Mat3& other)
	{
		return Mat3(
			collumns[0] + other.collumns[0],
			collumns[1] + other.collumns[1],
			collumns[2] + other.collumns[2]
		);
	}

	Mat3 Mat3::operator-(const Mat3& other)
	{
		return Mat3(
			collumns[0] - other.collumns[0],
			collumns[1] - other.collumns[1],
			collumns[2] - other.collumns[2]
		);
	}

	Mat3 Mat3::operator*(const Mat3& other)
	{
		Mat3 res(0);
		int i, j, k;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				res(i, j) = 0;
				for (k = 0; k < 3; k++)
					res(i, j) += (*this)(i, k) * other(k, j);
			}
		}
		return res;
	}

	Mat3 Mat3::operator*(const float& other)
	{
		return Mat3(
			collumns[0] * other,
			collumns[1] * other,
			collumns[2] * other
		);
	}

	float Mat3::operator[](int index) const
	{
		if (index < 0 || index >= 9) throw std::out_of_range("Index out of range");

		int col = index / 3;
		int row = index % 3;

		return collumns[col][row];
	}

	float Mat3::operator()(int row, int col) const
	{
		if (row < 0 || row >= 3 || col < 0 || col >= 3) throw std::out_of_range("Index out of range");
		return collumns[col][row];
	}

	float& Mat3::operator()(int row, int col)
	{
		if (row < 0 || row >= 3 || col < 0 || col >= 3) throw std::out_of_range("Index out of range");
		return collumns[col][row];
	}
}