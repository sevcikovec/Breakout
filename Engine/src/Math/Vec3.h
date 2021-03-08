#pragma once
#include <ostream>

namespace Engine {

	struct Vec3 {
	public:
		Vec3();
		Vec3(float x, float y, float z);

		Vec3(float num);

		Vec3(const Vec3& other);

		void Add(const Vec3& other);

		void Sub(const Vec3& other);

		void Mul(const float& scalar);

		void Div(const float& scalar);

		float Mag();

		float MagSq();

		Vec3 Opposite();

		Vec3 Normalized();

		void Normalize();

		Vec3 operator+(const Vec3& other);

		Vec3 operator-(const Vec3& other);

		Vec3 operator*(const float& other);

		float operator[](int) const;

		float& operator[](int);

		static float Dot(const Vec3& a, const Vec3& b);

		static Vec3 Cross(const Vec3& a, const Vec3& b);

		friend std::ostream& operator<<(std::ostream& os, Vec3 const& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
		}

		float x;
		float y;
		float z;
	};
}