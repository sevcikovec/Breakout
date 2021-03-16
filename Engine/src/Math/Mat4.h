#pragma once
#include "Vec4.h"
#include "Mat3.h"
#include "ostream"

namespace Engine {

	struct Mat4 {
	public:
		Mat4(const float& i);
		//Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);
		//Mat4(const Mat4& other);
		Mat4(
			const float& x0, const float& y0, const float& z0, const float& w0,
			const float& x1, const float& y1, const float& z1, const float& w1,
			const float& x2, const float& y2, const float& z2, const float& w2,
			const float& x3, const float& y3, const float& z3, const float& w3
		);

		Mat4(
			const Mat3& mat3
		);

		static Mat4 GetTranslationMatrix(const Vec3& translation);

		//Mat4 Transpose();
		//Mat4 Inverse();
		//float Det();
		//Mat4 operator+(const Mat4& other);
		//Mat4 operator-(const Mat4& other);
		Mat4 operator*(const Mat4& other);
		//Mat4 operator*(const float& other);
		float operator[](int) const;

		float operator()(int row, int col) const;

		float& operator()(int row, int col);

		friend std::ostream& operator<<(std::ostream& os, const Mat4& v) {
			os << v[0] << ", " << v[3] << ", " << v[6] << std::endl;
			os << v[1] << ", " << v[4] << ", " << v[7] << std::endl;
			os << v[2] << ", " << v[5] << ", " << v[8] << std::endl;
			return os;
		}

		static Mat4 GetScaleMatrix(Vec3 scale) {
			return Mat4(
				scale.x, 0.0f,0.0f,0.0f,
				0.0f, scale.y, 0.0f,0.0f,
				0.0f,0.0f,scale.z, 0.0f,
				0.0f,0.0f,0.0f,1.0f
			);
		}

		const float* GetPtr() {
			return &collumns[0][0];
		}
		
	private:
		Vec4 collumns[4];
	};
}