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
		std::vector<ColliderObject> collisionObjects;

		std::vector<CollisionPair> collisionPairs;
	};
}