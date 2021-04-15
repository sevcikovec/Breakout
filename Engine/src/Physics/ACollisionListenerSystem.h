#pragma once
#include "../Scene/Systems.h"
#include "CollisionStructs.h"

namespace Engine {
	class ACollisionListenerSystem : public OnUpdateSystem {
	public:
		void Init(ECS* ecs) override {
			OnUpdateSystem::Init(ecs);
			SetEntities();
		}

		virtual void SetEntities() = 0;

		void Reset() {
			collisionEvents.clear();
		}

		void AddCollisionEvent(const CollisionEvent& event) {
			collisionEvents.push_back(event);
		}

	protected:
		template<typename... C>
		void SetCollisionEntityColliding() {
			signatureEntityColliding = ecs->GetSignature<C...>();
		}

		template<typename... C>
		void SetCollisionEntityOther() {
			signatureEntityOther = ecs->GetSignature<C...>();
		}

		std::vector<CollisionEvent> collisionEvents;

	private:
		Signature signatureEntityColliding;
		Signature signatureEntityOther;
	
	private:
		friend class CollisionEventDispatcherSystem;

		bool IsRequestedCollision(Signature colliding, Signature other) {
			return (signatureEntityColliding & colliding) == signatureEntityColliding &&
				(signatureEntityOther & other) == signatureEntityOther;
		}
	};
}