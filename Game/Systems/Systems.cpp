#include "Systems.h"
#include "../Components/PlayerComponent.h"
#include "../Components/BallComponent.h"
#include <Core/Input.h>
#include "Math/Coordinates.h"

void PlayerMovementSystem::Update(float ts) {
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

		transform.position = Engine::CylindricalToCartesian({ player.radius, transform.position.y, -transform.rotation.y });
	}
}

void BallSystem::Update(float ts) {
	auto view = ecs->GetView<BallComponent, Engine::TransformComponent>();
	while (view.MoveNext()) {
		auto& transform = view.GetComponent<Engine::TransformComponent>();
		auto& ball = view.GetComponent<BallComponent>();

		float rotDelta = ts * 30;

		Engine::Vec3 rotation{ 0 };
		if (Engine::Input::IsKeyDown(Engine::KeyCode::LEFT)) {
			rotation.y -= rotDelta;
		}
		if (Engine::Input::IsKeyDown(Engine::KeyCode::RIGHT)) {
			rotation.y += rotDelta;
		}
		if (Engine::Input::IsKeyDown(Engine::KeyCode::UP)) {
			rotation.x += rotDelta;
		}
		if (Engine::Input::IsKeyDown(Engine::KeyCode::DOWN)) {
			rotation.x -= rotDelta;
		}

		transform.rotation.Add(rotation);
	}
}