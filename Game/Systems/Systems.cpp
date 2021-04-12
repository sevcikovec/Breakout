#include "Systems.h"
#include "../Components/BreakoutComponents.h"
#include <Core/Input.h>
#include "Math/Coordinates.h"

using namespace Engine;

void PlayerMovementSystem::Update(float ts) {
	auto view = ecs->GetView<PlayerComponent, TransformComponent>();
	while (view.MoveNext())
	{
		auto& transform = view.GetComponent<TransformComponent>();
		auto& player = view.GetComponent<PlayerComponent>();

		float moveDelta = player.moveSpeed * ts * 30;

		Vec3 move{ 0 };
		float rotation = transform.rotation.y;
		if (Input::IsKeyDown(KeyCode::A)) {
			rotation -= moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::D)) {
			rotation += moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::W)) {
			//rotation.x += moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::S)) {
			//rotation.x -= moveDelta;
		}

		transform = TransformComponent::GetArchTransform(rotation, player.radius, transform.position.y);
	}
}

void BallSystem::Update(float ts) {
		
}

void BlockSystem::Update(float ts)
{
	auto view = ecs->GetView<BlockComponent, CollisionEvent>();
	while (view.MoveNext()) {
		auto& collisionEvent = view.GetComponent<CollisionEvent>();

		if (ecs->HasComponents<BallComponent>(collisionEvent.otherEntity)) {
			ecs->AddComponent<DestroyTag>(view.GetEntity());
		}
	}
}
