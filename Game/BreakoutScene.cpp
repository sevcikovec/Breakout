#include <cstdio>
#include "BreakoutScene.h"
#include "Scene/Entity.h"
#include "Systems/Systems.h"
#include "Utils/MeshGenerator.h"
#include "Renderer/Material.h"
#include "Physics/PhysicsComponents.h"
#include "Components/BallComponent.h"
#include "Components/PlayerComponent.h"

using namespace Engine;

BreakoutScene::BreakoutScene() : Scene("Main scene")
{
	// initialize scene

	// register systems
	RegisterSystem<PlayerMovementSystem>();
	RegisterSystem<BallSystem>();

	// add camera
	{
		Entity cameraEntity = CreateEntity("Main camera");
		auto& cameraTransform = cameraEntity.AddComponent<TransformComponent>();
		cameraTransform.position = { 0, 10,7.f };
		cameraTransform.rotation = { -60, 0,0 };
		
		Ref<Camera> camera = CreateRef<Camera>();
		camera->SetPerspective(0.785398f, 0.1f, 100.f);
		camera->SetViewport(640, 480);

		auto& cameraComponent = cameraEntity.AddComponent<CameraComponent>();
		cameraComponent.camera = camera;
		cameraComponent.primary = true;
	}


	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	auto shader = CreateRef<Shader>(vertexShader, fragmentShader);



	std::vector<float> vertices;
	std::vector<uint32_t> indices;	
	
	// generate ball
	{

		float radius = 0.25;
		MeshGenerator::GenerateSphere(radius, 20, 20, vertices, indices);
		auto ballMeshVAO = GetVertexArray(vertices, indices);

		auto ballMaterial = CreateRef<Material>();
		ballMaterial->SetShader(shader);
		ballMaterial->SetProperty("color", Vec3{ .7f, .0f, 0 });

		Entity ballEntity = CreateEntity("Ball");
		ballEntity.AddComponent<BallComponent>();
		auto& transform = ballEntity.AddComponent<TransformComponent>();
		transform.scale = { 1.f, 1.f, 1.f };
		transform.position = { 0.f,0.25f,0.f };
		transform.rotation = { 0.f,0.0f, 0.f };
		auto& mesh = ballEntity.AddComponent<MeshComponent>();
		mesh.material = ballMaterial;
		mesh.vao = ballMeshVAO;
		auto& platformAABB = ballEntity.AddComponent<AABB_local>();
		platformAABB.xMin = -radius;
		platformAABB.yMin = -radius;
		platformAABB.zMin = -radius;
		platformAABB.xMax =  radius;
		platformAABB.yMax =  radius;
		platformAABB.zMax =  radius;

		auto& velocity = ballEntity.AddComponent<VelocityComponent>();
		velocity.velocity.z = 1;
	}

	// generate player
	{
		auto playerArchMaterial = CreateRef<Material>();
		playerArchMaterial->SetShader(shader);
		playerArchMaterial->SetProperty("color", Vec3{ .0f, .0f, .7f });

		float innerRadius = 4.f;
		float outerRadius = 4.5f;
		float playerRadius = (innerRadius + outerRadius) / 2;
		MeshGenerator::GenerateArk(innerRadius, outerRadius, 45.f, 0.5f, 50, true, vertices, indices);
		auto playerMeshVAO = GetVertexArray(vertices, indices);
		CreatePlayerArch(playerArchMaterial, playerMeshVAO, 0, playerRadius);
		CreatePlayerArch(playerArchMaterial, playerMeshVAO, 120, playerRadius);
		CreatePlayerArch(playerArchMaterial, playerMeshVAO, 240, playerRadius);
	}
	
	// generate floor
	{
		auto platformMaterial = CreateRef<Material>();
		platformMaterial->SetShader(shader);
		platformMaterial->SetProperty("color", Vec3{ .7f, .5f, 0 });

		MeshGenerator::GenerateCircle(5, 50, vertices, indices);
		auto platformVAO = GetVertexArray(vertices, indices);
		Entity platformEntity = CreateEntity("Platform entity");
		auto& transform = platformEntity.AddComponent<TransformComponent>();
		transform.scale = { 1.f, 1.f, 1.f };
		transform.position = { 0.f,.0f,0.f };
		transform.rotation = { 0.f,0.0f, 0.f };
		auto& mesh = platformEntity.AddComponent<MeshComponent>();
		mesh.material = platformMaterial;
		mesh.vao = platformVAO;
		auto& platformAABB = platformEntity.AddComponent<AABB_local>();
		platformAABB.xMin = -5.f;
		platformAABB.xMax = 5.f;
		platformAABB.yMin = -0.2f;
		platformAABB.yMax = 0.f;
		platformAABB.zMin = -5.f;
		platformAABB.zMax = 5.f;
	}
	
}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Scene::OnUpdate(frameTimeMS);
}

Ref<VertexArray> BreakoutScene::GetVertexArray(std::vector<float>& vertices, std::vector<uint32_t>& indices)
{
	auto vb1 = VertexBuffer::Create(vertices.data(), vertices.size());
	vb1->SetLayout(
		{
			{ LayoutShaderType::Float3 }
		});

	auto ib = IndexBuffer::Create(indices.data(), indices.size());

	auto vao = VertexArray::Create();
	vao->SetIndexBuffer(ib);
	vao->SetVertexBuffer(vb1);
	return vao;
}

void BreakoutScene::CreatePlayerArch(Engine::Ref<Engine::Material> playerArchMaterial, Engine::Ref<Engine::VertexArray> playerMeshVAO, float startingAngle, float radius)
{
	Entity playerArchEntity = CreateEntity("Player entity");
	auto& playerTransform = playerArchEntity.AddComponent<TransformComponent>();
	playerTransform.scale = { 1.f, 1.f, 1.f };
	playerTransform.position = { 0.f,.0f,0.f };
	playerTransform.rotation = { 0.f, startingAngle, 0.f };
	auto& playerMesh = playerArchEntity.AddComponent<MeshComponent>();
	playerMesh.material = playerArchMaterial;
	playerMesh.vao = playerMeshVAO;

	// todo calculate automatically AABB
	auto& playerAABB = playerArchEntity.AddComponent<AABB_local>();
	playerAABB.xMin = -0.6f;
	playerAABB.xMax = 0.25f;
	playerAABB.yMin = -0.f;
	playerAABB.yMax = .5f;
	playerAABB.zMin = -1.8f;
	playerAABB.zMax = 1.8f;


	auto& player = playerArchEntity.AddComponent<PlayerComponent>();
	player.radius = radius;
}

