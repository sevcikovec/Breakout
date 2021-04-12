#pragma once
#include "APhysicsSystem.h"

namespace Engine {
	class CollisionEventsSystem : public APhysicsSystem {
	public:
		void Update(float ts) {
			//reset all events
			{
				std::vector<EntityID> entities;
				auto& eventsView = ecs->GetView<CollisionEvent>();
				while (eventsView.MoveNext()) {
					entities.push_back(eventsView.GetEntity());
				}
				for (auto ent : entities) {
					ecs->RemoveComponent<CollisionEvent>(ent);
				}
			}



			auto& objects = pWorld->collisionObjects;
			auto& collisionPairs = pWorld->collisionPairs;

			for (size_t i = 0; i < collisionPairs.size(); i++)
			{
				auto& first = objects[collisionPairs[i].ColliderObjectAIndex];
				auto& second = objects[collisionPairs[i].ColliderObjectBIndex];

				/*
				auto& firstCollisionEvent = ecs->AddComponent<CollisionEvent>(first.entity);
				firstCollisionEvent.collisionNormal = collisionPairs[i].collisionNormal;
				firstCollisionEvent.otherEntity = second.entity;

				auto& secondCollisionEvent = ecs->AddComponent<CollisionEvent>(second.entity);
				secondCollisionEvent.collisionNormal = collisionPairs[i].collisionNormal * -1;
				secondCollisionEvent.otherEntity = first.entity;
				*/
			}


			// update entities with velocity
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