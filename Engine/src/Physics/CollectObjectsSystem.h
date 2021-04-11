#pragma once
#include "APhysicsSystem.h"

namespace Engine {
	class CollectObjectsSystem : public APhysicsSystem {
	public:
		void Update(float ts) {
			auto viewSphere = ecs->GetView<SphereCollider, AABB_local, TransformComponent>();
			while (viewSphere.MoveNext())
			{
				auto& transform = viewSphere.GetComponent<TransformComponent>();
				auto& localAABB = viewSphere.GetComponent<AABB_local>();
				auto entity = viewSphere.GetEntity();

				pWorld->AddColliderObject(ColliderObject{entity, AABB(localAABB, transform.GetTransformMatrix()), transform, ColliderType::sphere});
			}
			
			auto viewArch = ecs->GetView<ArchCollider, AABB_local, TransformComponent>();
			while (viewArch.MoveNext())
			{
				auto& transform = viewArch.GetComponent<TransformComponent>();
				auto& localAABB = viewArch.GetComponent<AABB_local>();
				auto entity = viewArch.GetEntity();

				pWorld->AddColliderObject(ColliderObject{ entity, AABB(localAABB, transform.GetTransformMatrix()), transform , ColliderType::arch});
			}
		}

	};
}