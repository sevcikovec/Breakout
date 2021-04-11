#pragma once
#include "../Scene/Systems.h"
#include "PhysicsWorld.h"

namespace Engine {
	class CollectObjectsSystem : public OnUpdateSystem {
	public:
		void SetPhysicsWorld(PhysicsWorld* pWorld) {
			this->pWorld = pWorld;
		}

		void Update(float ts) override {
			auto view = ecs->GetView<AABB_local, TransformComponent>();
			while (view.MoveNext())
			{
				auto& transform = view.GetComponent<TransformComponent>();
				auto& localAABB = view.GetComponent<AABB_local>();
				auto entity = view.GetEntity();

				pWorld->AddColliderObject(ColliderObject{entity, AABB(localAABB, transform.GetTransformMatrix()), transform});
			}
		}
	private:
		PhysicsWorld* pWorld;
	};
}