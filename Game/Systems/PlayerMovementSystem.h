#pragma once
#include "../Components/PlayerComponent.h"
#include <Scene/Systems.h>
#include <Core/Input.h>
class PlayerMovementSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override {
		auto view = ecs->GetView<PlayerComponent, Engine::TransformComponent>();
		while (view.MoveNext())
		{
			auto& transform = view.GetComponent<Engine::TransformComponent>();
			auto& player = view.GetComponent<PlayerComponent>();
			
			float moveDelta = player.moveSpeed * ts * 30;

			Engine::Vec3 move{ 0 };
			Engine::Vec3 rotation{ 0 };
			if (Engine::Input::IsKeyDown(Engine::KeyCode::A)) {
				rotation.y -= moveDelta;
			}
			if (Engine::Input::IsKeyDown(Engine::KeyCode::D)) {
				rotation.y += moveDelta;
			}
			if (Engine::Input::IsKeyDown(Engine::KeyCode::W)) {
				//rotation.x += moveDelta;
			}
			if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) {
				//rotation.x -= moveDelta;
			}
			
			transform.rotation.Add(rotation);

			auto angle = transform.rotation.y;
			auto newPositionCircleX = player.radius * cos(Engine::ToRadians(angle));
			auto newPositionCircleY = player.radius * sin(Engine::ToRadians(angle));

			transform.position.x = newPositionCircleX;
			transform.position.z = -newPositionCircleY;

			//rotate for no reason
			//transform.rotation.y += ts * 20;
			//transform.rotation.x += ts * 20;

		}
	}
};