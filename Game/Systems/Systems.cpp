#include "Systems.h"
#include "../Components/BreakoutComponents.h"
#include <Core/Input.h>
#include "Math/Coordinates.h"
#include <iostream>

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

		Vec3 prevPos = transform.position;

		transform = TransformComponent::GetArchTransform(rotation, player.radius, transform.position.y);

		player.movementDelta = transform.position - prevPos;
	}
}

void BallSystem::Update(float ts) {
	auto view = ecs->GetView<BallComponent, TransformComponent, Rigidbody>();
	while (view.MoveNext()) {
		auto& transform = view.GetComponent<TransformComponent>();
		auto& rb = view.GetComponent<Rigidbody>();
		rb.velocity.Normalize();
		rb.velocity.Mul(5);
	}
}

void BlockSystem::SetEntities()
{
	SetCollisionEntityColliding<BlockComponent>();
	SetCollisionEntityOther<BallComponent>();	
}

void BlockSystem::Update(float ts)
{
	for (const auto& collisionEvent : collisionEvents) {		
		if (!ecs->HasComponent<DestroyTag>(collisionEvent.collidingEntity))
			ecs->AddComponent<DestroyTag>(collisionEvent.collidingEntity);
	}
}

void BounceSystem::SetEntities()
{
	SetCollisionEntityColliding<BallComponent, Rigidbody>();
	SetCollisionEntityOther<PlayerComponent>();	
}

void BounceSystem::Update(float ts)
{
	// collision reaction with bounce
	for (const auto& collisionEvent : collisionEvents) {
		
		auto& rb = ecs->GetComponent<Rigidbody>(collisionEvent.collidingEntity);
		auto& transform = ecs->GetComponent<TransformComponent>(collisionEvent.collidingEntity);

		auto& playerComponent = ecs->GetComponent<PlayerComponent>(collisionEvent.otherEntity);
		
		rb.velocity += playerComponent.movementDelta.Normalized() * 2;
	}
}

void LightMoveSystem::Update(float ts)
{
	auto view = ecs->GetView<LightComponent, TransformComponent>();
	while (view.MoveNext()) {
		auto& transform = view.GetComponent<TransformComponent>();

		Vec3 move{ 0 };
		float moveDelta = 4 * ts;
		if (Input::IsKeyDown(KeyCode::LEFT)) {
			move.x -= moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::RIGHT)) {
			move.x += moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::UP)) {
			move.z -= moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::DOWN)) {
			move.z += moveDelta;
		}

		transform.position += move;
	}
}
