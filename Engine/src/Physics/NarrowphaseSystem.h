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
				else if (objects[aIndex].colliderType == ColliderType::sphere && objects[bIndex].colliderType == ColliderType::sphere) {
					isColliding = ResolveSphereSphereCollisions(aIndex, bIndex, collisionPair);
				}
				else if (objects[aIndex].colliderType == ColliderType::arch && objects[bIndex].colliderType == ColliderType::arch) {
					isColliding = ResolveArchArchCollisions(aIndex, bIndex, collisionPair);
				}
				else if (objects[aIndex].colliderType == ColliderType::arch && objects[bIndex].colliderType == ColliderType::box) {
					isColliding = ResolveArchBoxCollisions(aIndex, bIndex, collisionPair);
				}
				else if (objects[aIndex].colliderType == ColliderType::box && objects[bIndex].colliderType == ColliderType::arch) {
					isColliding = ResolveArchBoxCollisions(bIndex, aIndex, collisionPair);
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

			Vec2 spherePolar = CartesianToPolar({ sphereObject.transform.position.x, -sphereObject.transform.position.z });
			Vec2 centerArchPolar = CartesianToPolar({ archObject.transform.position.x, -archObject.transform.position.z });

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
					collisionPair.penetrationDepth = overlappingDistanceFrontBack;
				}
				// side collision
				else {
					collisionPair.collisionNormal = sidesNormal;
					collisionPair.penetrationDepth = overlappingDistanceSides;
				}


				collisionPair.ColliderObjectAIndex = sphereIndex;
				collisionPair.ColliderObjectBIndex = archIndex;

				return true;
			}
			

			return false;
		}
		
		bool ResolveSphereSphereCollisions(const size_t& sphereAIndex, const size_t& sphereBIndex, CollisionPair& collisionPair) {
			auto & objects = pWorld->collisionObjects;

			auto& sphereAObject = objects[sphereAIndex];
			auto& sphereBObject = objects[sphereBIndex];

			auto& colliderA = ecs->GetComponent<SphereCollider>(sphereAObject.entity);
			auto& colliderB = ecs->GetComponent<SphereCollider>(sphereBObject.entity);
			
			float minkowskiSphereARadius = colliderA.radius + colliderB.radius;

			float distance = (sphereBObject.transform.position - sphereAObject.transform.position).Mag();
			if (distance < minkowskiSphereARadius) {
				collisionPair.ColliderObjectAIndex = sphereAIndex;
				collisionPair.ColliderObjectBIndex = sphereBIndex;
				float e = 0.f;// 0.00000001f; // added epsilon if spheres on the same position
				collisionPair.collisionNormal = ((sphereAObject.transform.position + e) - sphereBObject.transform.position).Normalized();
				collisionPair.penetrationDepth = minkowskiSphereARadius - distance;
				return true;
			}

			return false;
		}

		bool ResolveArchArchCollisions(const size_t& archIndex1, const size_t& archIndex2, CollisionPair& collisionPair) {
			auto& objects = pWorld->collisionObjects;

			auto& archObject1 = objects[archIndex1];
			auto& archObject2 = objects[archIndex2];

			auto& archCollider1 = ecs->GetComponent<ArchCollider>(archObject1.entity);
			auto& archCollider2 = ecs->GetComponent<ArchCollider>(archObject2.entity);
			
			// arches aren't intersecting
			if (archCollider1.outerRadius < archCollider2.innerRadius || archCollider2.outerRadius < archCollider1.innerRadius) return false;

			// testing side overlap
			float halfWidthMinkowski = (archCollider1.angleWidth / 2) + (archCollider2.angleWidth / 2);

			Vec2 centerArch1Polar = CartesianToPolar({ archObject1.transform.position.x, -archObject1.transform.position.z });
			Vec2 centerArch2Polar = CartesianToPolar({ archObject2.transform.position.x, -archObject2.transform.position.z });
			
			float centerArch1Angle = fmod(centerArch1Polar.y, 360) + 360;
			float centerArch2Angle = fmod(centerArch2Polar.y, 360) + 360;

			float angleSideDifference = abs(centerArch1Angle - centerArch2Angle);
			float sideOverlap = halfWidthMinkowski - angleSideDifference;
			bool sideColliding = false;
			
			if (sideOverlap > 0.001f) {
				sideColliding = true;
			}
			if (!sideColliding) return false;



			// testing Y overlap
			float minkowskyY = archCollider1.height / 2.f + archCollider2.height / 2.f;
			
			float arch1Ycenter = archObject1.transform.position.y + archCollider1.height / 2.f;
			float arch2Ycenter = archObject2.transform.position.y + archCollider2.height / 2.f;


			float arch1TopBottomOverlap = minkowskyY - abs(arch1Ycenter - arch2Ycenter);
			bool isCollisingTopBottom = false;
			Vec3 topBottomNormal(0,1,0);
			if (arch1TopBottomOverlap > 0) {
				isCollisingTopBottom = true;
				if (archObject1.transform.position.y < archObject2.transform.position.y)
					topBottomNormal = topBottomNormal * -1;
			}

			if (isCollisingTopBottom) {
				
				collisionPair.collisionNormal = topBottomNormal;
				collisionPair.penetrationDepth = arch1TopBottomOverlap;
				
				collisionPair.ColliderObjectAIndex = archIndex1;
				collisionPair.ColliderObjectBIndex = archIndex2;
				return true;
			}
			
			return false;
		}

		bool ResolveArchBoxCollisions(const size_t& archIndex, const size_t& boxIndex, CollisionPair& collisionPair) {
			auto& objects = pWorld->collisionObjects;

			auto& archObject = objects[archIndex];
			auto& boxObject = objects[boxIndex];

			auto& archCollider = ecs->GetComponent<ArchCollider>(archObject.entity);
			auto& boxCollider = ecs->GetComponent<BoxCollider>(boxObject.entity);

			bool isCollidingTopBottom = false;
			Vec3 topBottomNormal(0, 1, 0);

			float minkowskyY = archCollider.height / 2.f + boxCollider.size.y / 2.f;
			float archYCenter = archObject.transform.position.y + archCollider.height / 2.f;
			float boxYCenter = boxObject.transform.position.y;

			float boxOverlap = minkowskyY - abs(archYCenter - boxYCenter);

			if (boxOverlap > 0.001) {
				if (archYCenter < boxYCenter) {
					topBottomNormal *= -1;
				}
				isCollidingTopBottom = true;
			}


			if (isCollidingTopBottom) {

				collisionPair.collisionNormal = topBottomNormal;
				collisionPair.penetrationDepth = boxOverlap;

				collisionPair.ColliderObjectAIndex = archIndex;
				collisionPair.ColliderObjectBIndex = boxIndex;
				return true;
			}

			return false;
		}

	};
}