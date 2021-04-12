#pragma once
#include "../Math/Vec3.h"
#include "../ECS/ComponentManager.h"

namespace Engine {
	struct VelocityComponent {
		Vec3 velocity;
	};

	///
	/// AABB in model space
	///
	struct AABB_local {
		float xMin, yMin, zMin;
		float xMax, yMax, zMax;

		Vec3 color{ 0,1,0 };
	};

	struct SphereCollider {
		float radius;
	};

	struct ArchCollider {
		Vec3 center{0};
		float innerRadius;
		float outerRadius;
	};

	struct CollisionEvent {
		EntityID otherEntity;
		Vec3 collisionNormal;
	};
}