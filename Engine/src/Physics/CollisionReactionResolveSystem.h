#pragma once
#include "APhysicsSystem.h"

namespace Engine {
	class CollisionReactionResolveSystem : public APhysicsSystem {

	public:
		void Update(float ts) override {

			auto& collisionPairs = pWorld->collisionPairs;
			auto& objects = pWorld->collisionObjects;

			// update entities that has been displaced
			for (size_t i = 0; i < collisionPairs.size(); i++)
			{
				auto& first = objects[collisionPairs[i].ColliderObjectAIndex];
				auto& second = objects[collisionPairs[i].ColliderObjectBIndex];

				if (first.colliderType == ColliderType::sphere && second.colliderType == ColliderType::sphere) {
					auto& firstDisplacement = collisionPairs[i].firstObjectDisplacement;
					firstDisplacement.Mul(0.5f);
					first.transform.position = first.transform.position + firstDisplacement;

					auto& secondDisplacement = firstDisplacement;
					secondDisplacement.Mul(-1);
					second.transform.position = second.transform.position + secondDisplacement;
				}
				else {
					auto& firstDisplacement = collisionPairs[i].firstObjectDisplacement;
					first.transform.position = first.transform.position + firstDisplacement;
				}

			}

			// update transforms
			for (size_t i = 0; i < objects.size(); i++)
			{
				auto& transform = ecs->GetComponent<TransformComponent>(objects[i].entity);
				transform = objects[i].transform;
			}


			// collision reaction with bounce
			auto viewVelocity = ecs->GetView<CollisionEvent, VelocityComponent>();
			while (viewVelocity.MoveNext()) {
				auto& velocity = viewVelocity.GetComponent<VelocityComponent>();
				auto& collision = viewVelocity.GetComponent<CollisionEvent>();

				float currentSpeed = velocity.velocity.Mag();

				Vec3 newVelocity = Vec3::Reflect(velocity.velocity, collision.collisionNormal);
				newVelocity.Normalize();
				newVelocity.Mul(currentSpeed);
				velocity.velocity = newVelocity;
			}
		}
	};
}