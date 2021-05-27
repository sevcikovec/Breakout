#pragma once
#include "APhysicsSystem.h"
#include <iostream>

namespace Engine {
	class CollisionReactionResolveSystem : public APhysicsSystem {

	public:
		void Update(float ts) override {

			auto& collisionPairs = pWorld->collisionPairs;
			auto& objects = pWorld->collisionObjects;

			size_t it = 3;
			for (size_t k = 0; k < it; k++)
			for (size_t i = 0; i < collisionPairs.size(); i++)
			{
				auto& first = objects[collisionPairs[i].ColliderObjectAIndex];
				auto& second = objects[collisionPairs[i].ColliderObjectBIndex];


				float penetrationDepth = collisionPairs[i].penetrationDepth;

				Vec3 collisionNormal = collisionPairs[i].collisionNormal;

				Vec3 relativeVelocity(0);

				float e = 1;
				float mass1 = 0;
				float mass2 = 0;
				if (first.hasRigidbody && second.hasRigidbody) {
					relativeVelocity = first.rigidbody.velocity - second.rigidbody.velocity;
					e = fmax(first.rigidbody.restitution, second.rigidbody.restitution);
					mass1 = first.rigidbody.mass;
					mass2 = second.rigidbody.mass;
				}
				else if (first.hasRigidbody && !second.hasRigidbody){
					relativeVelocity = first.rigidbody.velocity;
					e = first.rigidbody.restitution;
					mass1 = first.rigidbody.mass;
				}
				else if (!first.hasRigidbody && second.hasRigidbody) {
					relativeVelocity = second.rigidbody.velocity * -1;
					e = second.rigidbody.restitution;
					mass2 = second.rigidbody.mass;
				}

				float normalVelocity = Vec3::Dot(relativeVelocity, collisionNormal);

				float j = 0;
				if (normalVelocity > 0) continue;

				float invMass1 = mass1 == 0 ? 0 : 1 / mass1;
				float invMass2 = mass2 == 0 ? 0 : 1 / mass2;

				float b = 5.f;
				float slop = 0.005f;
				
				{
					j = -(1 + e) * normalVelocity;
					j /= invMass1 + invMass2;
				}

				j += fmax(penetrationDepth - slop, 0) * b;

				if (first.hasRigidbody)
					first.rigidbody.velocity += collisionNormal * j * invMass1;
				if (second.hasRigidbody)
					second.rigidbody.velocity -= collisionNormal * j * invMass2;

			}
	

			// update transforms and velocities
			for (auto& object : objects)
			{
				auto& transform = ecs->GetComponent<TransformComponent>(object.entity);
				transform = object.transform;

				if (object.hasRigidbody) {
					auto& rb = ecs->GetComponent<Rigidbody>(object.entity);
					rb = object.rigidbody;


					if (rb.constraintMoveX) rb.velocity.x = 0;
					if (rb.constraintMoveY) rb.velocity.y = 0;
					if (rb.constraintMoveZ) rb.velocity.z = 0;

					transform.position += rb.velocity * ts;
				}
			}
		}
	};
}