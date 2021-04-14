#pragma once
#include "../Math/Vec3.h"
#include "../ECS/ComponentManager.h"

namespace Engine {
	struct VelocityComponent {
		Vec3 velocity;
	
		bool constraintMoveX = false;
		bool constraintMoveY = false;
		bool constraintMoveZ = false;
	};


	///
	/// AABB in model space
	///
	struct AABB_local {
		float xMin, yMin, zMin;
		float xMax, yMax, zMax;
	};

	struct SphereCollider {
		float radius;
	};

	struct ArchCollider {
		Vec3 center{0};
		float innerRadius;
		float outerRadius;
		float angleWidth;
	};

	struct CollisionEvent {
		EntityID otherEntity;
		Vec3 collisionNormal;
	};
}