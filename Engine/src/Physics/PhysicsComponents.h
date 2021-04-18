#pragma once
#include "../Math/Vec3.h"
#include "../ECS/ComponentManager.h"

namespace Engine {
	struct Rigidbody {
		Vec3 velocity;
	
		bool constraintMoveX = false;
		bool constraintMoveY = false;
		bool constraintMoveZ = false;

		bool useGravity = true;
		
		float mass = 1;

		float restitution = .3f;

		float isKinematic = false;

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
		float height;
	};

	struct BoxCollider {
		Vec3 size;
	};
}