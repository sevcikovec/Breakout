#include "Mat4.h"

namespace Engine {
	Mat4::Mat4(const float& i)
	{
		this->collumns[0] = Vec4(i   , 0.0f, 0.0f, 0.0f);
		this->collumns[1] = Vec4(0.0f, i   , 0.0f, 0.0f);
		this->collumns[2] = Vec4(0.0f, 0.0f, i	 , 0.0f);
		this->collumns[3] = Vec4(0.0f, 0.0f, 0.0f, i   );
	}

	Mat4::Mat4(
		const float& x0, const float& y0, const float& z0, const float& w0,
		const float& x1, const float& y1, const float& z1, const float& w1,
		const float& x2, const float& y2, const float& z2, const float& w2,
		const float& x3, const float& y3, const float& z3, const float& w3)
	{
		this->collumns[0] = Vec4(x0, x1, x2, x3);
		this->collumns[1] = Vec4(y0, y1, y2, y3);
		this->collumns[2] = Vec4(z0, z1, z2, z3);
		this->collumns[3] = Vec4(w0, w1, w2, w3);
	}

	Mat4::Mat4(const Mat3& mat3) 
	{
		this->collumns[0] = Vec4(mat3(0, 0)	, mat3(0, 1), mat3(0, 2), 0.0f);
		this->collumns[1] = Vec4(mat3(1, 0)	, mat3(1, 1), mat3(1, 2), 0.0f);
		this->collumns[2] = Vec4(mat3(2, 0)	, mat3(2, 1), mat3(2, 2), 0.0f);
		this->collumns[3] = Vec4(0.0f		, 0.0f		, 0.0f		, 1.0f);
	}

	Mat4 Mat4::GetTranslationMatrix(const Vec3& translation)
	{
		Mat4 res(1);
		res.collumns[3][0] += translation.x;
		res.collumns[3][1] += translation.y;
		res.collumns[3][2] += translation.z;
		return res;
	}

		Mat4 Engine::Mat4::operator*(const Mat4& other)
	{
		Mat4 res(0);
		int i, j, k;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				res(i, j) = 0;
				for (k = 0; k < 4; k++)
					res(i, j) += (*this)(i, k) * other(k, j);
			}
		}
		return res;
	}

	float Mat4::operator()(int row, int col) const
	{
		if (row < 0 || row >= 4 || col < 0 || col >= 4) throw std::out_of_range("Index out of range");
		return collumns[col][row];
	}

	float& Mat4::operator()(int row, int col)
	{
		if (row < 0 || row >= 4 || col < 0 || col >= 4) throw std::out_of_range("Index out of range");
		return collumns[col][row];
	}

	float Mat4::operator[](int index) const
	{
		if (index < 0 || index >= 16) throw std::out_of_range("Index out of range");

		int col = index / 4;
		int row = index % 4;

		return collumns[col][row];
	}
}