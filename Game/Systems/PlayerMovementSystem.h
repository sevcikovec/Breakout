#pragma once
#include "../Components/PlayerComponent.h"
#include <Scene/Systems.h>
#include <Core/Input.h>
class PlayerMovementSystem : public Engine::BaseSystem<Engine::TransformComponent, PlayerComponent> {
	void Update(float ts) override {
		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& transform = e.GetComponent<Engine::TransformComponent>();
			auto& player = e.GetComponent<PlayerComponent>();
			
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
				rotation.x += moveDelta;
			}
			if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) {
				rotation.x -= moveDelta;
			}
			 
			//rotate for no reason
			//transform.rotation.y += ts * 20;
			//transform.rotation.x += ts * 20;

			transform.rotation.Add(rotation);
		}
	}
};