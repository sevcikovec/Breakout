#pragma once
#include "../Scene/Systems.h"
#include "../Scene/Components.h"
#include "PhysicsComponents.h"

namespace Engine {
	class DynamicsSystem : public OnUpdateSystem {
	public:
		void Update(float ts) override {
			auto view = ecs->GetView<VelocityComponent, TransformComponent>();
			while (view.MoveNext())
			{
				auto& transform = view.GetComponent<TransformComponent>();
				auto& velocity = view.GetComponent<VelocityComponent>();

				transform.position = transform.position + velocity.velocity * ts;
			}
		}
	};
}