#pragma once
#include "APhysicsSystem.h"
#include "../Scene/Components.h"
#include "PhysicsComponents.h"

namespace Engine {
	class DynamicsSystem : public APhysicsSystem {
	public:
		void Update(float ts) override {

			auto& objects = pWorld->collisionObjects;
			Vec3 gravity(0, -9.87f, 0);
			for (auto& object : objects)
			{
				if (!object.hasRigidbody) continue;


				Vec3 force(0);
				if (object.rigidbody.useGravity)
					force += gravity * object.rigidbody.mass;

				object.rigidbody.velocity += force / object.rigidbody.mass * ts;

				//float speed = velocity.velocity.Mag();

				//if (velocity.constraintMoveX) velocity.velocity.x = 0;
				//if (velocity.constraintMoveY) velocity.velocity.y = 0;
				//if (velocity.constraintMoveZ) velocity.velocity.z = 0;

				//velocity.velocity.Normalize();
				//velocity.velocity.Mul(speed);

				//object.transform.position += object.velocity.velocity * ts;
				
			}
		}
	};
}