#pragma once
#include "APhysicsSystem.h"
namespace Engine {
	class BroadphaseSystem : public APhysicsSystem {
	public:
		void Update(float ts) override {
			auto& objects = pWorld->collisionObjects;

			auto& possibleCollisions = pWorld->possibleCollisions;

			// naive checking all pairs
			for (size_t i = 0; i < objects.size(); i++)
			{
				for (size_t j = i+1; j < objects.size(); j++)
				{
					if (AABB::IsOverlapping(objects[i].aabb, objects[j].aabb)) {
						possibleCollisions.push_back(PossibleCollisionPair{i, j});
					}
				}
			}
		}
	};
}