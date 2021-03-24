#pragma once
#include "../Math/Vec3.h"

namespace Engine {
	struct VelocityComponent {
		Vec3 velocity;
	};

	///
	/// AABB in model space
	///
	struct AABB {
		float xMin, yMin, zMin;
		float xMax, yMax, zMax;
	};
}