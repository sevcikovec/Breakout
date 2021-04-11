#pragma once
#include "Ray.h"
#include "CollisionStructs.h"
#include <vector>
namespace Engine {
	class PhysicsWorld {
	public:
		void Reset();
		bool CastRay(Ray ray);

		void AddColliderObject(ColliderObject object);
	private:
		friend class BroadphaseSystem;
		friend class NarrowphaseSystem;
		friend class CollisionEventsSystem;

		std::vector<ColliderObject> collisionObjects;

		std::vector<PossibleCollisionPair> possibleCollisions;

		std::vector<CollisionPair> collisionPairs;
	};
}