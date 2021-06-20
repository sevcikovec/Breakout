#include "Systems.h"
#include "../Components/BreakoutComponents.h"
#include <Core/Input.h>
#include <Math/Coordinates.h>
#include <Utils/MeshGenerator.h>
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
		auto& ballComponent = view.GetComponent<BallComponent>();
		auto& rb = view.GetComponent<Rigidbody>();
		rb.velocity.Normalize();
		rb.velocity.Mul(ballComponent.speed);
	}
}

void BlockSystem::SetEntities()
{
	SetCollisionEntityColliding<BlockComponent>();
	SetCollisionEntityOther<BallComponent>();	
}

void BlockSystem::Update(float ts)
{
	int scoreChange = 0;

	for (const auto& collisionEvent : collisionEvents) {		
		auto& block = ecs->GetComponent<BlockComponent>(collisionEvent.collidingEntity);
		block.durability -= 1;

		if (block.durability <= 0 && !ecs->HasComponent<DestroyTag>(collisionEvent.collidingEntity)) {
			ecs->AddComponent<DestroyTag>(collisionEvent.collidingEntity);
			scoreChange += block.score;
		}
	}

	if (scoreChange > 0) {

		auto managerView = ecs->GetView<GameManagerComponent>();
		if (managerView.MoveNext()) {
			auto& managerComponent = managerView.GetComponent<GameManagerComponent>();
			managerComponent.totalScore += scoreChange;

			auto& scoreChangedevent = ecs->AddComponent<ScoreChangedEvent>(ecs->CreateEntity());
			scoreChangedevent.score = managerComponent.totalScore;
		}
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
		if (Input::IsKeyDown(KeyCode::C)) {
			move.y -= moveDelta;
		}
		if (Input::IsKeyDown(KeyCode::V)) {
			move.y += moveDelta;
		}

		transform.position += move;
	}
}

void OuterEdgeSystem::SetEntities()
{
	SetCollisionEntityColliding<DestroyOnLeavingCircleTag>();
	SetCollisionEntityOther<OuterEdgeComponent>();
}

void OuterEdgeSystem::Update(float ts)
{
	// collision reaction with outer edge
	for (const auto& collisionEvent : collisionEvents) {

		auto& BallComponent = ecs->GetComponent<Rigidbody>(collisionEvent.collidingEntity);

		if (!ecs->HasComponent<DestroyTag>(collisionEvent.collidingEntity))
			ecs->AddComponent<DestroyTag>(collisionEvent.collidingEntity);
	}
}

void BallSpawnerSystem::Init(ECS* ecs) {
	OnUpdateSystem::Init(ecs);

	radius = 0.2;
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	MeshGenerator::GenerateSphere(radius, 20, 20, vertices, indices);

	auto vb1 = VertexBuffer::Create(vertices.data(), vertices.size());
	vb1->SetLayout(
		{
			{ LayoutShaderType::Float3, LayoutShaderType::Float3 }
		});

	auto ib = IndexBuffer::Create(indices.data(), indices.size());

	ballVAO = VertexArray::Create();
	ballVAO->SetIndexBuffer(ib);
	ballVAO->SetVertexBuffer(vb1);


	ballMaterial = CreateRef<Material>();
	ballMaterial->SetProperty("color", Vec3{ .7f, .7f, 0 });
}

void BallSpawnerSystem::Update(float ts)
{
	auto managerView = ecs->GetView<GameManagerComponent>();
	if (managerView.MoveNext()) {
		auto& managerComponent = managerView.GetComponent<GameManagerComponent>();
		if (managerComponent.isGameOver) return;

		auto ballView = ecs->GetView<BallComponent, TagComponent>();
		if (!ballView.MoveNext()) {
			// no ball remaining, remove life
			managerComponent.playerLives -= 1;

			auto& livesChengedEvent = ecs->AddComponent<LivesChangedEvent>(ecs->CreateEntity());
			livesChengedEvent.lives = managerComponent.playerLives;

			if (managerComponent.playerLives > 0) {
				CreateBall({ 0, 0, 3 }, { 0.1f, 0, -2 });
			}
			else {
				managerComponent.isGameOver = true;
				auto& livesChengedEvent = ecs->AddComponent<EndGameEvent>(ecs->CreateEntity());
			}
			
		}
	}
}

void BallSpawnerSystem::SetShader(Engine::Ref<Engine::Shader> shader)
{
	ballMaterial->SetShader(shader);
}

void BallSpawnerSystem::CreateBall(Engine::Vec3 position, Engine::Vec3 velocity)
{
	Entity ballEntity(ecs->CreateEntity(), ecs);
	TagComponent& tag = ballEntity.AddComponent<TagComponent>();
	ballEntity.AddComponent<BallComponent>();
	auto& transform = ballEntity.AddComponent<TransformComponent>();
	position.y = radius;
	transform.position = position;

	auto& mesh = ballEntity.AddComponent<MeshComponent>();
	mesh.material = ballMaterial;
	mesh.vao = ballVAO;
	auto& platformAABB = ballEntity.AddComponent<AABB_local>();
	platformAABB.xMin = -radius;
	platformAABB.yMin = -radius;
	platformAABB.zMin = -radius;
	platformAABB.xMax = radius;
	platformAABB.yMax = radius;
	platformAABB.zMax = radius;

	auto& rbComponent = ballEntity.AddComponent<Rigidbody>();
	rbComponent.velocity = velocity;
	rbComponent.constraintMoveY = true;
	rbComponent.useGravity = false;
	rbComponent.restitution = 1.f;
	rbComponent.mass = 0.2f;

	auto& sphereCollider = ballEntity.AddComponent<SphereCollider>();
	sphereCollider.radius = radius;

	ballEntity.AddComponent<DestroyOnLeavingCircleTag>();
}

void ShowInfoSystem::Update(float ts)
{
	auto guiView = ecs->GetView<GUIReferencesComponent>();
	if (!guiView.MoveNext())
		return;
	auto& guiInfoComponent = guiView.GetComponent<GUIReferencesComponent>();

	auto livesEventView = ecs->GetView<LivesChangedEvent>();
	if (livesEventView.MoveNext()) {
		auto& livesChangedEvent = livesEventView.GetComponent<LivesChangedEvent>();

		auto livesEventEntity = livesEventView.GetEntity();
		ecs->AddComponent<DestroyTag>(livesEventEntity);

		if (ecs->HasComponent<TextComponent>(guiInfoComponent.livesTextEntity)) {
			auto& scoreText = ecs->GetComponent<TextComponent>(guiInfoComponent.livesTextEntity);
			scoreText.text->SetText("Lives: " + std::to_string(livesChangedEvent.lives), 50);
		}
	}
	
	 
	auto scoreEventView = ecs->GetView<ScoreChangedEvent>();
	if (scoreEventView.MoveNext()) {
		auto& scoreChangedEvent = scoreEventView.GetComponent<ScoreChangedEvent>();
		
		auto entity = scoreEventView.GetEntity();
		ecs->AddComponent<DestroyTag>(entity);

		if (ecs->HasComponent<TextComponent>(guiInfoComponent.scoreTextEntity)) {
			auto& scoreText = ecs->GetComponent<TextComponent>(guiInfoComponent.scoreTextEntity);
			scoreText.text->SetText("Score: " + std::to_string(scoreChangedEvent.score), 50);
		}
	}
}

void TransformAnimationSystem::Update(float ts)
{
	auto animationView = ecs->GetView<TransformComponent, TransformAnimationComponent>();
	while (animationView.MoveNext()) {
		auto& transform = animationView.GetComponent<TransformComponent>();
		auto& animation = animationView.GetComponent<TransformAnimationComponent>();

		auto toEnd = animation.endPosition - transform.position;
		auto rotationDifference = animation.endRotation - transform.rotation;

		float currentTimeStep = ts > animation.remainingDurationSec ? animation.remainingDurationSec : ts;

		auto timeDelta = currentTimeStep / animation.remainingDurationSec;

		auto moveDelta = toEnd * timeDelta;
		auto rotationDelta = rotationDifference * timeDelta;


		transform.position += moveDelta;
		transform.rotation += rotationDelta;

		animation.remainingDurationSec -= currentTimeStep;

		if (animation.remainingDurationSec <= 0) {
			ecs->RemoveComponent<TransformAnimationComponent>(animationView.GetEntity());
		}
	}
}

void CameraMovementController::Update(float ts)
{
	auto view = ecs->GetView<CameraAnimationComponent>();
	while (view.MoveNext()) {
		auto& cameraAnimationComponent = view.GetComponent<CameraAnimationComponent>();

		if (!Input::IsKeyDown(cameraAnimationComponent.key)) {
			continue;
		}

		if (ecs->HasComponent<TransformComponent>(cameraAnimationComponent.cameraEntity)) {
			TransformAnimationComponent transformAnimation;
			transformAnimation.endPosition = cameraAnimationComponent.targetPosition;
			transformAnimation.endRotation = cameraAnimationComponent.targetRotation;
			transformAnimation.remainingDurationSec = 0.5f;
			if (!ecs->HasComponent<TransformAnimationComponent>(cameraAnimationComponent.cameraEntity)) {
				auto& c = ecs->AddComponent<TransformAnimationComponent>(cameraAnimationComponent.cameraEntity);
				c = transformAnimation;
			}

		}
	}
}

void EndGameSystem::Update(float ts)
{
	auto view = ecs->GetView<EndGameEvent>();
	if (view.MoveNext()){
		ecs->AddComponent<DestroyTag>(view.GetEntity());

		auto textShader = Engine::Shader::GetShader("text");

		auto font = Engine::FontLoader::GetFont("default");
		auto text = CreateRef<Text>(font);
		text->SetText("Game Over", 75);

		EntityID endGameTextEntity = ecs->CreateEntity();

		// get canvas
		EntityID canvasEntityId;
		auto canvasView = ecs->GetView<CanvasComponent>();
		if (!view.MoveNext()) {
			canvasEntityId = canvasView.GetEntity();
		}

		auto& textTransform = ecs->AddComponent<RectTransform>(endGameTextEntity);
		textTransform.parentCanvasEntity = canvasEntityId;
		textTransform.position = { 125, 0, 0 };
		textTransform.size = { 0, 0 };
		textTransform.scale = { 1, 1, 1 };


		auto material = CreateRef<Material>();
		material->SetShader(textShader);
		material->SetProperty("color", Vec3{ 0 });
		material->SetTexture(font->GetAtlasTexture());

		auto& textComponent = ecs->AddComponent<TextComponent>(endGameTextEntity);
		textComponent.material = material;
		textComponent.text = text;
	}
}
