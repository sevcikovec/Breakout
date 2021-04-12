#pragma once
#include "APhysicsSystem.h"
#include "../Math/Coordinates.h"

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

			collisionPair.ColliderObjectAIndex = sphereIndex;
			collisionPair.ColliderObjectBIndex = archIndex;

			float sphereDistFromCenter = sphereObject.transform.position.Mag();
			
			float sphereRadius = sphereCollider.radius;
			float minkowskiInnerRadius = archCollider.innerRadius - sphereRadius;
			float minkowskiOuterRadius = archCollider.outerRadius + sphereRadius;
		

			float overlappingDistanceFrontBack = 0;
			bool collidingFrontBack = sphereDistFromCenter <= minkowskiOuterRadius && sphereDistFromCenter >= minkowskiInnerRadius;

			Vec3 frontBackNormal(0);

			if (collidingFrontBack) {
				// if colliding on inner side
				if (fabs(sphereDistFromCenter - minkowskiInnerRadius) < fabs(sphereDistFromCenter - minkowskiOuterRadius)) {
					frontBackNormal = sphereObject.transform.position;
					frontBackNormal.Mul(-1);
					frontBackNormal.y = 0;
					frontBackNormal.Normalize();
					overlappingDistanceFrontBack = sphereDistFromCenter - minkowskiInnerRadius;
				}
				// if colliding on outer side
				else {
					frontBackNormal = sphereObject.transform.position;
					frontBackNormal.y = 0;
					frontBackNormal.Normalize();
					overlappingDistanceFrontBack = minkowskiOuterRadius - sphereDistFromCenter;
					collidingFrontBack = true;
				}
			}
			

			// if not overlapping with front/back size, there cannot be a colision
			if (!collidingFrontBack) return false;

			Vec2 spherePolar = CartesianToPolar({ sphereObject.transform.position.x, sphereObject.transform.position.z });
			Vec2 centerArchPolar = CartesianToPolar({ archObject.transform.position.x, archObject.transform.position.z });

			float currentSpherePosCircumference = 2 * PI * sphereDistFromCenter;
			float angleChangeForSphereRadius = (sphereRadius / currentSpherePosCircumference) * 360.f;

			float halfWidthMinkowski = (archCollider.angleWidth / 2) + angleChangeForSphereRadius;
			Vec2 ccwBorderPolar(spherePolar.x, centerArchPolar.y + halfWidthMinkowski);
			Vec2 cwBorderPolar(spherePolar.x, centerArchPolar.y - halfWidthMinkowski);
			float ccwBorderAnglePosWOffset = fmod(ccwBorderPolar.y, 360) + 360;
			float cwBorderAnglePosWOffset = fmod(cwBorderPolar.y, 360) + 360;

			float sphereAnglePosWOffset = fmod(spherePolar.y, 360) + 360;
			float archCenterPosWOffset = fmod(centerArchPolar.y, 360) + 360;

			float overlappingDistanceSides = 0;

			bool collidingSides = sphereAnglePosWOffset <= ccwBorderAnglePosWOffset && sphereAnglePosWOffset >= cwBorderAnglePosWOffset;

			float angleDiffFromCenter = sphereAnglePosWOffset - archCenterPosWOffset;

			Vec3 sidesNormal(0);

			if (collidingSides) {
				// sphere is on ccw side
				if (sphereAnglePosWOffset - archCenterPosWOffset > 0) {
					Vec3 ccwBorderPos = CylindricalToCartesian({ ccwBorderPolar.x, sphereObject.transform.position.y, ccwBorderPolar.y });

					overlappingDistanceSides = (ccwBorderPos - sphereObject.transform.position).Mag();

					auto forward = sphereObject.transform.position;
					forward.y = 0;
					forward.Mul(-1);
					sidesNormal = Vec3::Right(forward, { 0,1,0 });
					sidesNormal.Normalize();
				}
				// sphere is on cw side
				else {
					Vec3 cwBorderPos = CylindricalToCartesian({ cwBorderPolar.x, sphereObject.transform.position.y, cwBorderPolar.y });

					overlappingDistanceSides = (sphereObject.transform.position - cwBorderPos).Mag();

					auto forward = sphereObject.transform.position;
					forward.y = 0;
					sidesNormal = Vec3::Right(forward, { 0,1,0 });
					sidesNormal.Normalize();
				}

			}
			
			if (collidingSides && collidingFrontBack) {
				// front/back collision
				if (overlappingDistanceFrontBack < overlappingDistanceSides) {
					collisionPair.collisionNormal = frontBackNormal;
					collisionPair.firstObjectDisplacement = frontBackNormal * overlappingDistanceFrontBack;
				}
				// side collision
				else {
					collisionPair.collisionNormal = sidesNormal;
					collisionPair.firstObjectDisplacement = sidesNormal * -overlappingDistanceSides;
				}

				return true;
			}
			

			return false;
		}
	};
}