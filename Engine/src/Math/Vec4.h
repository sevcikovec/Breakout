#pragma once
#include <ostream>

namespace Engine {

	struct Vec4 {
	public:
		Vec4();
		Vec4(float x, float y, float z, float w);

		//Vec4(float num);
		/*
		Vec4(const Vec4& other);

		void Add(const Vec4& other);

		void Sub(const Vec4& other);

		void Mul(const float& scalar);

		void Div(const float& scalar);

		float Mag();

		float MagSq();

		Vec4 Opposite();

		Vec4 Normalized();

		void Normalize();

		Vec4 operator+(const Vec4& other);

		Vec4 operator-(const Vec4& other);

		Vec4 operator*(const float& other);
		*/
		float operator[](int) const;

		float& operator[](int);
		/*
		static float Dot(const Vec4& a, const Vec4& b);

		static Vec4 Cross(const Vec4& a, const Vec4& b);
		*/
		friend std::ostream& operator<<(std::ostream& os, Vec4 const& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
		}

		float x, y, z, w;
	};
}