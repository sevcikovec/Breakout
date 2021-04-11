#pragma once
#include "Vec2.h"
#include "Vec3.h"
namespace Engine {
	Vec2 CartesianToPolar(Vec2 cartesian);
	Vec2 PolarToCartesian(Vec2 polar);

	Vec3 CylindricalToCartesian(Vec3 cylindrical);
	Vec3 CartesianToCylindrical(Vec3 cartesian);

	Vec3 SphericalToCartesian(float radius, float inclination, float azimuth);
}