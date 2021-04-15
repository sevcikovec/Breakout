#pragma once
#include "APhysicsSystem.h"
#include "ACollisionListenerSystem.h"

namespace Engine {
	class CollisionEventDispatcherSystem : public APhysicsSystem {
	public:
		
		void AddListener(const Ref<ACollisionListenerSystem>& listener) {
			listeners.push_back(listener);
		}

		void Update(float ts) {
			auto& objects = pWorld->collisionObjects;
			auto& collisionPairs = pWorld->collisionPairs;

			for (const auto& collision : collisionPairs) {
				auto& objectA = objects[collision.ColliderObjectAIndex];
				auto& objectB = objects[collision.ColliderObjectBIndex];
				for (const auto& listener : listeners) {
					if (listener->IsRequestedCollision(objectA.entitySignature, objectB.entitySignature)) {
						listener->AddCollisionEvent({ objectA.entity, objectB.entity, collision.collisionNormal });
					}
					if (listener->IsRequestedCollision(objectB.entitySignature, objectA.entitySignature)) {
						listener->AddCollisionEvent({ objectB.entity, objectA.entity, collision.collisionNormal * -1 });
					}
				}
			}

			for (const auto& listener : listeners) {
				listener->Update(ts);
				listener->Reset();
			}
		}
	private:
		std::vector<Ref<ACollisionListenerSystem>> listeners;
	};
}