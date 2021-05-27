#include <cstdio>
#include "BreakoutScene.h"
#include "Scene/Entity.h"
#include "Systems/Systems.h"
#include "Utils/MeshGenerator.h"
#include "Renderer/Material.h"
#include "Physics/PhysicsComponents.h"
#include "Components/BreakoutComponents.h"
#include "Renderer/Texture.h"

using namespace Engine;

BreakoutScene::BreakoutScene() : Scene("Main scene")
{
	// initialize scene
	// register systems
	auto playerSystem = RegisterSystem<PlayerMovementSystem>();
	auto ballSystem = RegisterSystem<BallSystem>();

	auto blockSystem = RegisterOnCollisionSystem<BlockSystem>();
	auto ballBounceSystem = RegisterOnCollisionSystem<BounceSystem>();
	auto outerEdgeSystem = RegisterOnCollisionSystem<OuterEdgeSystem>();


	auto lightMoveSysem = RegisterSystem<LightMoveSystem>();

	// add camera
	{
		Entity cameraEntity = CreateEntity("Main camera");
		auto& cameraTransform = cameraEntity.AddComponent<TransformComponent>();
		cameraTransform.position = { 0, 20,13.f };
		cameraTransform.rotation = { -60.f, 0,0 };
		//cameraTransform.position = { 0, 30,0.f };
		//cameraTransform.rotation = { -90.f, 0,0 };
		//cameraTransform.position = { 0, 0,20.f };
		//cameraTransform.rotation = { 0.f, 0,0 };
		Ref<Camera> camera = CreateRef<Camera>();
		camera->SetPerspective(.6f, 0.1f, 100.f);
		camera->SetViewport(640, 480);

		auto& cameraComponent = cameraEntity.AddComponent<CameraComponent>();
		cameraComponent.camera = camera;
		cameraComponent.primary = true;
	}

	// add light
	{
		Entity lightEntity = CreateEntity("Main Light");
		auto& transform = lightEntity.AddComponent<TransformComponent>();
		transform.position = { 0,4,0 };
		auto& light = lightEntity.AddComponent<LightComponent>();
		light.diffuse = {1.f,1.f,1.f };
		light.ambient = { 0.5f,0.5f,0.5f };
		light.specular = { 1 };
	}


	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	auto shader = CreateRef<Shader>(vertexShader, fragmentShader);



	std::vector<float> vertices;
	std::vector<uint32_t> indices;	
	
	// generate ball
	{
		
		float radius = 0.2;
		MeshGenerator::GenerateSphere(radius, 20, 20, vertices, indices);
		auto ballMeshVAO = GetVertexArray(vertices, indices, { LayoutShaderType::Float3, LayoutShaderType::Float3 });

		auto ballMaterial = CreateRef<Material>();
		ballMaterial->SetShader(shader);
		ballMaterial->SetProperty("color", Vec3{ .7f, .7f, 0 });

		auto ball1 = CreateBall(ballMaterial, ballMeshVAO, radius, { 0.f, radius, 3.0f }, { 0, 0, -2});
		
		//auto ball2 = CreateBall(ballMaterial, ballMeshVAO, radius, { -3.f, 0.1f, 0.3f }, { 2, 0, 0 });
		
		//CreateBall(ballMaterial, ballMeshVAO, radius, { 0.f, 0.1f, 3.4f }, { 6, 0, 2 });
		//CreateBall(ballMaterial, ballMeshVAO, radius, { 0.f, 0.1f, -3.4f }, { 6, 0, 2 });
		//CreateBall(ballMaterial, ballMeshVAO, radius, { 0.f, 0.1f, 0.4f }, { 6, 0, 2 });
		//CreateBall(ballMaterial, ballMeshVAO, radius, { 0.f, 0.1f, 1.4f }, { 6, 0, 2 });
		/*for (int i = 0; i < 45; i++) {
			CreateBall(ballMaterial, ballMeshVAO, radius, { ((rand() % 70) - 35) / 10.f, 0.1f, ((rand() % 70) - 35) / 10.f }, { 0, 0, 40 });
		}*/
	}


	
	// generate wall
	{
		auto material1 = CreateRef<Material>();
		material1->SetShader(shader);
		material1->SetProperty("color", Vec3{ .7f, .7f, .0f });
		auto material2 = CreateRef<Material>();
		material2->SetShader(shader);
		material2->SetProperty("color", Vec3{ .0f, .7f, .7f });

		float numberInRow = 12;
		int rows = 3;

		float innerRadius = 1.f;
		float outerRadius = 1.5f;
		float radius = (innerRadius + outerRadius) / 2;
		float widthAngle = 360/numberInRow;
		float height = .5f;
		MeshGenerator::GenerateArk(innerRadius, outerRadius, widthAngle, height, 10, true, vertices, indices);
		auto aabb = MeshGenerator::GetAABB(vertices);
		auto vertexArray = GetVertexArray(vertices, indices, { LayoutShaderType::Float3, LayoutShaderType::Float3 });

		//for (int i = rows-1; i >= 0; i--) {
		for (int i = 0; i < rows; i++) {
			for (size_t j = 0; j < numberInRow; j++)
			{
				auto& block = CreateBlockArch((j + i) % 2 ? material1 : material2, vertexArray, widthAngle, widthAngle * j, radius, innerRadius, outerRadius, height, height * i + i * 0.f -0.f, aabb);
				//if (i != 0)
				auto& rb = block.AddComponent<Rigidbody>();
				rb.restitution = .1f;
				rb.mass = 1.f;
				rb.constraintMoveX = true;
				rb.constraintMoveZ = true;
			}
		}
	}
	
	
	// generate outer wall
	{
		auto outerWallMaterial = CreateRef<Material>();
		outerWallMaterial->SetShader(shader);
		outerWallMaterial->SetProperty("color", Vec3{ .7f, .7f, .7f });

		float innerRadius = 7.5f;
		float outerRadius = 9.f;
		float playerRadius = (innerRadius + outerRadius) / 2;
		float angleWidth = 360.f;
		float height = 1.f;
		MeshGenerator::GenerateArk(innerRadius, outerRadius, angleWidth, height, 50, true, vertices, indices);
		auto aabb = MeshGenerator::GetAABB(vertices);
		auto playerMeshVAO = GetVertexArray(vertices, indices, { LayoutShaderType::Float3, LayoutShaderType::Float3 });
		auto& wall = CreateArch(outerWallMaterial, playerMeshVAO, angleWidth, 0, playerRadius, innerRadius, outerRadius, height, 0.0f, aabb);
		wall.RemoveComponent<MeshComponent>();
		auto& outerEdgeComponent = wall.AddComponent<OuterEdgeComponent>();
	}
	

	
	// generate player
	{
		auto playerArchMaterial = CreateRef<Material>();
		playerArchMaterial->SetShader(shader);
		playerArchMaterial->SetProperty("color", Vec3{ .7f, .0f, .0f });

		float innerRadius = 6.f;
		float outerRadius = 6.5f;
		float playerRadius = (innerRadius + outerRadius) / 2;
		float angleWidth = 40.f;
		float height = 0.5f;
		MeshGenerator::GenerateArk(innerRadius, outerRadius, angleWidth, height, 15, true, vertices, indices);
		auto aabb = MeshGenerator::GetAABB(vertices);
		auto playerMeshVAO = GetVertexArray(vertices, indices, { LayoutShaderType::Float3, LayoutShaderType::Float3 });
		auto& player1 = CreatePlayerArch(playerArchMaterial, playerMeshVAO, angleWidth, 0, playerRadius, innerRadius, outerRadius, height, aabb);
		auto& player2 = CreatePlayerArch(playerArchMaterial, playerMeshVAO, angleWidth, 120, playerRadius, innerRadius, outerRadius, height, aabb);
		auto& player3 = CreatePlayerArch(playerArchMaterial, playerMeshVAO, angleWidth, 240, playerRadius, innerRadius, outerRadius, height, aabb);
	}
	
	const char* textureFilename = "..\\..\\..\\..\\Engine\\resources\\Lenna.png";
	auto texture = CreateRef<Texture>(textureFilename);


	// generate floor
	{
		float radius = 7;
		auto platformMaterial = CreateRef<Material>();
		platformMaterial->SetShader(shader);
		platformMaterial->SetProperty("color", Vec3{ 1.f, 1.f, 1.f });
		platformMaterial->SetTexture(texture);

		MeshGenerator::GenerateCircle(radius, 50, vertices, indices);
		auto platformVAO = GetVertexArray(vertices, indices, { LayoutShaderType::Float3, LayoutShaderType::Float3, LayoutShaderType::Float2 });
		Entity platformEntity = CreateEntity("Platform entity");
		auto& transform = platformEntity.AddComponent<TransformComponent>();
		transform.scale = { 1.f, 1.f, 1.f };
		transform.position = { 0.f,0.-0.2f,0.f };
		transform.rotation = { 0.f,0.0f, 0.f };
		auto& mesh = platformEntity.AddComponent<MeshComponent>();
		mesh.material = platformMaterial;
		mesh.vao = platformVAO;
		auto& platformAABB = platformEntity.AddComponent<AABB_local>();
		platformAABB.xMin = -radius;
		platformAABB.xMax = radius;
		platformAABB.yMin = -0.2f;
		platformAABB.yMax = 0.2f;
		platformAABB.zMin = -radius;
		platformAABB.zMax = radius;

		auto& boxCollider = platformEntity.AddComponent<BoxCollider>();
		boxCollider.size = { radius*2, 0.4f, radius*2 };
	}
	
}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Scene::OnUpdate(frameTimeMS);
}

Ref<VertexArray> BreakoutScene::GetVertexArray(std::vector<float>& vertices, std::vector<uint32_t>& indices, const BufferLayout& bufferLayout)
{
	auto vb1 = VertexBuffer::Create(vertices.data(), vertices.size());
	vb1->SetLayout(
		{
			bufferLayout
		});

	auto ib = IndexBuffer::Create(indices.data(), indices.size());

	auto vao = VertexArray::Create();
	vao->SetIndexBuffer(ib);
	vao->SetVertexBuffer(vb1);
	return vao;
}


Entity BreakoutScene::CreatePlayerArch(Engine::Ref<Engine::Material> playerArchMaterial, Engine::Ref<Engine::VertexArray> playerMeshVAO, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, float height, AABB_local aabb)
{
	Entity entity = CreateArch(playerArchMaterial, playerMeshVAO, angleWidth, startingAngle, radius, innerRadius, outerRadius, height, 0, aabb);

	auto& playerEntity = entity.AddComponent<PlayerComponent>();
	playerEntity.radius = radius;

	return entity;
}

Engine::Entity BreakoutScene::CreateArch(Engine::Ref<Engine::Material> material, Engine::Ref<Engine::VertexArray> vertexArray, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, float height, float yPos, Engine::AABB_local aabb)
{
	Entity entity = CreateEntity("Arch entity");
	auto& playerTransform = entity.AddComponent<TransformComponent>();
	playerTransform = TransformComponent::GetArchTransform(startingAngle, radius, yPos);
	auto& playerMesh = entity.AddComponent<MeshComponent>();
	playerMesh.material = material;
	playerMesh.vao = vertexArray;

	// todo calculate automatically AABB
	auto& playerAABB = entity.AddComponent<AABB_local>();
	playerAABB = aabb;

	auto& archCollider = entity.AddComponent<ArchCollider>();
	archCollider.innerRadius = innerRadius;
	archCollider.outerRadius = outerRadius;
	archCollider.angleWidth = angleWidth;
	archCollider.height = height;

	return entity;
}

Engine::Entity BreakoutScene::CreateBlockArch(Engine::Ref<Engine::Material> playerArchMaterial, Engine::Ref<Engine::VertexArray> playerMeshVAO, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, float height, float yPos, Engine::AABB_local aabb)
{
	Entity entity = CreateArch(playerArchMaterial, playerMeshVAO, angleWidth, startingAngle, radius, innerRadius, outerRadius, height, yPos, aabb);

	auto& block = entity.AddComponent<BlockComponent>();
	block.durability = 1;

	return entity;
}

Engine::Entity BreakoutScene::CreateBall(Engine::Ref<Engine::Material> material, Engine::Ref<Engine::VertexArray> vertexArray, float radius, Engine::Vec3 position, Engine::Vec3 velocity)
{
	Entity ballEntity = CreateEntity("Ball");
	ballEntity.AddComponent<BallComponent>();
	auto& transform = ballEntity.AddComponent<TransformComponent>();
	transform.position = position;
	
	auto& mesh = ballEntity.AddComponent<MeshComponent>();
	mesh.material = material;
	mesh.vao = vertexArray;
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

	return ballEntity;
}

