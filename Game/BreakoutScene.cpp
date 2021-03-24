#include "BreakoutScene.h"
#include <Scene\Entity.cpp>
#include <cstdio>
#include "Scripts/TestScript.cpp"
#include "Systems/PlayerMovementSystem.h"
#include "Utils/MeshGenerator.h"
#include <Renderer\Material.h>
#include <Physics\PhysicsComponents.h>

using namespace Engine;

BreakoutScene::BreakoutScene() : Scene("Main scene")
{
	// initialize scene

	// register system
	RegisterSystem<PlayerMovementSystem>();



	// add entities
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


	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	auto shader = CreateRef<Shader>(vertexShader, fragmentShader);

	auto platformMaterial = CreateRef<Material>();
	platformMaterial->SetShader(shader);
	platformMaterial->SetProperty("color", Vec3{ .7f, .5f, 0 });

	auto playerArchMaterial = CreateRef<Material>();
	playerArchMaterial->SetShader(shader);
	playerArchMaterial->SetProperty("color", Vec3{ .0f, .0f, .7f });
	
	

	std::vector<float> vertices;
	std::vector<uint32_t> indices;	


	MeshGenerator::GenerateCircle(5, 50, vertices, indices);
	auto platformVAO = GetVertexArray(vertices, indices);
	
	MeshGenerator::GenerateArk(4.f, 4.5f, 45.f, 0.5f, 10, true, vertices, indices);
	auto playerMeshVAO = GetVertexArray(vertices, indices);

	Entity platformEntity = CreateEntity("Platform entity");
	auto& transform = platformEntity.AddComponent<TransformComponent>();
	transform.scale = { 1.f, 1.f, 1.f };
	transform.position = { 0.f,.0f,0.f };
	transform.rotation= { 0.f,0.0f, 0.f};
	auto& mesh = platformEntity.AddComponent<MeshComponent>();
	mesh.material = platformMaterial;
	mesh.vao = platformVAO;
	auto& platformAABB = platformEntity.AddComponent<AABB>();
	platformAABB.xMin = -5.f;
	platformAABB.xMax = 5.f;
	platformAABB.yMin = -0.2f;
	platformAABB.yMax = 0.f;
	platformAABB.zMin = -5.f;
	platformAABB.zMax = 5.f;


	Entity playerArchEntity = CreateEntity("Player entity");
	auto& playerTransform = playerArchEntity.AddComponent<TransformComponent>();
	playerTransform.scale = { 1.f, 1.f, 1.f };
	playerTransform.position = { 0.f,.0f,0.f };
	playerTransform.rotation = { 0.f,0.0f, 0.f };
	auto& playerMesh = playerArchEntity.AddComponent<MeshComponent>();
	playerMesh.material = playerArchMaterial;
	playerMesh.vao = playerMeshVAO;

	// todo calculate automatically AABB
	auto& playerAABB = playerArchEntity.AddComponent<AABB>();
	playerAABB.xMin = -0.6f;
	playerAABB.xMax = 0.25f;
	playerAABB.yMin = -0.f;
	playerAABB.yMax = .5f;
	playerAABB.zMin = -1.8f;
	playerAABB.zMax = 1.8f;


	auto& player = playerArchEntity.AddComponent<PlayerComponent>();
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

