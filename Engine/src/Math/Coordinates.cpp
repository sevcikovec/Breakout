#include "Coordinates.h"
#include "Quaternion.h"

namespace Engine {
	Vec2 CartesianToPolar(Vec2 cartesian)
	{
		float x = cartesian.x;
		float y = cartesian.y;
		return {
			sqrt(x * x + y * y),
			atan2(y, x)
		};
	}
	Vec2 PolarToCartesian(Vec2 polar)
	{
		return {
			polar.x * cos(ToRadians(polar.y)),
			polar.x * sin(ToRadians(polar.y))
		};
	}
	Vec3 CylindricalToCartesian(Vec3 cylindrical)
	{
		return {
			cylindrical.x * cos(ToRadians(cylindrical.z)),
			cylindrical.y,
			cylindrical.x * sin(ToRadians(cylindrical.z))
		};
	}
	Vec3 CartesianToCylindrical(Vec3 cartesian)
	{
		float x = cartesian.x;
		float y = cartesian.y;
		float z = cartesian.z;
		return {
			sqrt(x * x + z * z),
			y,
			atan2(z, x)
		};
	}
}