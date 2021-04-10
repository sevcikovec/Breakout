#pragma once
#include "../Scene/Entity.h"
#include "../Math/Vec3.h"
namespace Engine {
	class CollisionPair {
	public:
		Entity collidingA;
		Entity collidingB;
		Vec3 collisionPoint;
	};
}