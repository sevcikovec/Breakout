#include "Vec3.h"
#include <math.h>
#include <iostream>

Vec3::Vec3()
{
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3::Vec3(float num)
{
	x = num;
	y = num;
	z = num;
}

Vec3::Vec3(const Vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vec3::Add(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vec3::Sub(const Vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vec3::Mul(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vec3::Div(const float& scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float Vec3::Mag()
{
	return sqrt(x*x + y*y + z*z);
}

float Vec3::MagSq()
{
	return x * x + y * y + z * z;
}

Vec3 Vec3::Opposite()
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::Normalized()
{
	Vec3 other = *this;
	other.Normalize();
	return other;
}

void Vec3::Normalize()
{
	float mag = Mag();
	Div(mag);
}

Vec3 Vec3::operator+(const Vec3& other)
{
	Vec3 res(*this);
	res.Add(other);
	return res;
}

Vec3 Vec3::operator-(const Vec3& other)
{
	Vec3 res(*this);
	res.Sub(other);
	return res;
}

Vec3 Vec3::operator*(const float& other)
{
	Vec3 res(*this);
	res.Mul(other);
	return res;
}

float Vec3::operator[](int index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("Index out of range");
		break;
	}
}

float& Vec3::operator[](int index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("Index out of range");
		break;
	}
}

float Vec3::Dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::Cross(const Vec3& a, const Vec3& b)
{
	Vec3 cross(0);
	cross.x = a.y * b.z - a.z * b.y;
	cross.y = a.z * b.x - a.x * b.z;
	cross.z = a.x * b.y - a.y * b.x;
	return cross;
}
