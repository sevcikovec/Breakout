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

				bool hasRigidbody = ecs->HasComponent<Rigidbody>(entity);

				pWorld->AddColliderObject(ColliderObject{
					entity,
					ecs->GetEntitySignature(entity),
					AABB(localAABB, transform.GetTransformMatrix()),
					transform,
					ColliderType::sphere,
					hasRigidbody,
					hasRigidbody ? viewSphere.GetComponent<Rigidbody>() : Rigidbody()
					});
			}
			
			auto viewArch = ecs->GetView<ArchCollider, AABB_local, TransformComponent>();
			while (viewArch.MoveNext())
			{
				auto& transform = viewArch.GetComponent<TransformComponent>();
				auto& localAABB = viewArch.GetComponent<AABB_local>();
				auto entity = viewArch.GetEntity();

				bool hasRigidbody = ecs->HasComponent<Rigidbody>(entity);

				pWorld->AddColliderObject(ColliderObject{
					entity,
					ecs->GetEntitySignature(entity),
					AABB(localAABB, transform.GetTransformMatrix()),
					transform,
					ColliderType::arch,
					hasRigidbody,
					hasRigidbody ? viewArch.GetComponent<Rigidbody>() : Rigidbody()
					});
			}

			auto viewBox = ecs->GetView<BoxCollider, AABB_local, TransformComponent>();
			while (viewBox.MoveNext())
			{
				auto& transform = viewBox.GetComponent<TransformComponent>();
				auto& localAABB = viewBox.GetComponent<AABB_local>();
				auto entity = viewBox.GetEntity();

				bool hasRigidbody = ecs->HasComponent<Rigidbody>(entity);

				pWorld->AddColliderObject(ColliderObject{
					entity,
					ecs->GetEntitySignature(entity),
					AABB(localAABB, transform.GetTransformMatrix()),
					transform,
					ColliderType::box,
					hasRigidbody,
					hasRigidbody ? viewBox.GetComponent<Rigidbody>() : Rigidbody()
					});
			}
		}

	};
}