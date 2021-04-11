#pragma once
#include "APhysicsSystem.h"

namespace Engine {
	class NarrowphaseSystem : public APhysicsSystem {

	public:
		void Update(float ts) override {
			auto& objects = pWorld->collisionObjects;

			auto& possibleCollisions = pWorld->possibleCollisions;

			auto& collisionPairs = pWorld->collisionPairs;

			// sphere to arch collision
			for (size_t i = 0; i < possibleCollisions.size(); i++)
			{
				auto aIndex = possibleCollisions[i].ColliderObjectAIndex;
				auto bIndex = possibleCollisions[i].ColliderObjectBIndex;

				CollisionPair collisionPair;
				bool isColliding = false;
				if (objects[aIndex].colliderType == ColliderType::sphere && objects[bIndex].colliderType == ColliderType::arch) {
					isColliding = ResolveSphereArchCollisions(aIndex, bIndex, collisionPair);
				}
				else if (objects[bIndex].colliderType == ColliderType::sphere && objects[aIndex].colliderType == ColliderType::arch) {
					isColliding = ResolveSphereArchCollisions(bIndex, aIndex, collisionPair);
				}

				if (isColliding) {
					collisionPairs.push_back(collisionPair);
				}
				
			}
		}

	private:
		bool ResolveSphereArchCollisions(const size_t& sphereIndex, const size_t& archIndex, CollisionPair& collisionPair) {
			auto& objects = pWorld->collisionObjects;

			auto& sphereObject = objects[sphereIndex];
			auto& archObject = objects[archIndex];

			auto& sphereCollider = ecs->GetComponent<SphereCollider>(sphereObject.entity);
			auto& archCollider = ecs->GetComponent<ArchCollider>(archObject.entity);

			float sphereDistFromCenter = sphereObject.transform.position.Mag();

			// TODO check side hits
			// if colliding on inner side
			if (sphereDistFromCenter + sphereCollider.radius >= archCollider.innerRadius &&
				sphereDistFromCenter - sphereCollider.radius <= archCollider.outerRadius) {
				collisionPair.ColliderObjectAIndex = sphereIndex;
				collisionPair.ColliderObjectBIndex = archIndex;
				collisionPair.collisionNormal = sphereObject.transform.position * -1;
				collisionPair.collisionNormal.y = 0;
				collisionPair.collisionNormal.Normalize();
				return true;
			}
			// if colliding on outer side
			if (sphereDistFromCenter + sphereCollider.radius <= archCollider.innerRadius &&
				sphereDistFromCenter - sphereCollider.radius >= archCollider.outerRadius) {
				collisionPair.ColliderObjectAIndex = sphereIndex;
				collisionPair.ColliderObjectBIndex = archIndex;
				collisionPair.collisionNormal = sphereObject.transform.position;
				collisionPair.collisionNormal.y = 0;
				collisionPair.collisionNormal.Normalize();
				return true;
			}

			return false;
		}
	};
}