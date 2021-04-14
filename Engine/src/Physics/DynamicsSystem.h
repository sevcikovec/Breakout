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

				float speed = velocity.velocity.Mag();
				if (velocity.constraintMoveX) velocity.velocity.x = 0;
				if (velocity.constraintMoveY) velocity.velocity.y = 0;
				if (velocity.constraintMoveZ) velocity.velocity.z = 0;

				velocity.velocity.Normalize();
				velocity.velocity.Mul(speed);

				transform.position = transform.position + velocity.velocity * ts;
			}
		}
	};
}