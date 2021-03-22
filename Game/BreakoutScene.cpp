#include "BreakoutScene.h"
#include <Scene\Entity.cpp>
#include <cstdio>
#include "Scripts/TestScript.cpp"
#include "Systems/PlayerMovementSystem.h"
#include "Utils/MeshGenerator.h"
#include <Renderer\Material.h>

BreakoutScene::BreakoutScene() : Engine::Scene("Main scene")
{
	// initialize scene
	RegisterSystem<PlayerMovementSystem>();
	/*
	std::string test = "test string";
	for (int i = 0; i < 100; i++) {
		CreateEntity(test + std::to_string(i));
	}*/

	/*
	auto& scriptComponent = entity.AddComponent<Engine::ScriptComponent>();
	scriptComponent.Bind<TestScript>();
	scriptComponent.InstantiateScript(&scriptComponent);

	auto testScript = static_cast<TestScript*>(scriptComponent.Instance);
	testScript->OnCreate();
	
	testScript->OnStateChanged = [this, entity]() {
		std::cout << " Callback on state changed" << std::endl;
		DestroyEntity(entity);
	};
	*/
	


	Engine::Entity cameraEntity = CreateEntity("Main camera");
	

	auto& cameraTransform = cameraEntity.AddComponent<Engine::TransformComponent>();
	cameraTransform.position = { 0, 10,7.f };
	cameraTransform.rotation = { -60, 0,0 };

	Engine::Ref<Engine::Camera> camera = Engine::CreateRef<Engine::Camera>();
	camera->SetPerspective(0.785398f, 0.1f, 100.f);
	camera->SetViewport(640, 480);

	auto& cameraComponent = cameraEntity.AddComponent<Engine::CameraComponent>();
	cameraComponent.camera = camera;
	cameraComponent.primary = true;


	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	auto shader = Engine::CreateRef<Engine::Shader>(vertexShader, fragmentShader);
	auto platformMaterial = Engine::CreateRef<Engine::Material>();
	platformMaterial->SetShader(shader);
	platformMaterial->SetProperty("color", Engine::Vec3{ .7f, .5f, 0 });

	auto playerArchMaterial = Engine::CreateRef<Engine::Material>();
	playerArchMaterial->SetShader(shader);
	playerArchMaterial->SetProperty("color", Engine::Vec3{ .0f, .0f, .7f });
	
	
	/*
	std::vector<float> vertices{

		-0.5f,0.5f,-0.5f,   0.0f, 0.0f, 0.0f,//Point A 0
		-0.5f,0.5f,0.5f,    0.0f, 0.0f, 1.0f,//Point B 1
		0.5f,0.5f,-0.5f,    0.0f, 1.0f, 0.0f,//Point C 2
		0.5f,0.5f,0.5f,     0.0f, 1.0f, 1.0f,//Point D 3

		-0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,//Point E 4
		-0.5f,-0.5f,0.5f,   1.0f, 0.0f, 1.0f,//Point F 5
		0.5f,-0.5f,-0.5f,   1.0f, 1.0f, 0.0f,//Point G 6
		0.5f,-0.5f,0.5f,    1.0f, 1.0f, 1.0f //Point H 7

	};
	std::vector<uint32_t> indices {
		//Above ABC,BCD
		0, 1, 2,
		1, 2, 3,

		//Following EFG,FGH
		4, 5, 6,
		5, 6, 7,
		//Left ABF,AEF
		0, 1, 5,
		0, 4, 5,
		//Right side CDH,CGH
		2, 3, 7,
		2, 6, 7,
		//ACG,AEG
		0, 2, 6,
		0, 4, 6,
		//Behind BFH,BDH
		1, 5, 7,
		1, 3, 7
	};
	*/

	std::vector<float> vertices;
	std::vector<uint32_t> indices;	


	Engine::MeshGenerator::GenerateCircle(5, 50, vertices, indices);
	auto platformVAO = GetVertexArray(vertices, indices);
	
	Engine::MeshGenerator::GenerateArk(4.f, 4.5f, 45.f, 0.5f, 10, true, vertices, indices);
	auto playerMeshVAO = GetVertexArray(vertices, indices);

	Engine::Entity platformEntity = CreateEntity("Platform entity");
	auto& transform = platformEntity.AddComponent<Engine::TransformComponent>();
	transform.scale = { 1.f, 1.f, 1.f };
	transform.position = { 0.f,.0f,0.f };
	transform.rotation= { 0.f,0.0f, 0.f};
	auto& mesh = platformEntity.AddComponent<Engine::MeshComponent>();
	mesh.material = platformMaterial;
	mesh.vao = platformVAO;
	
	Engine::Entity playerArchEntity = CreateEntity("Player entity");
	auto& playerTransform = playerArchEntity.AddComponent<Engine::TransformComponent>();
	playerTransform.scale = { 1.f, 1.f, 1.f };
	playerTransform.position = { 0.f,.0f,0.f };
	playerTransform.rotation = { 0.f,0.0f, 0.f };
	auto& playerMesh = playerArchEntity.AddComponent<Engine::MeshComponent>();
	playerMesh.material = playerArchMaterial;
	playerMesh.vao = playerMeshVAO;
	auto& player = playerArchEntity.AddComponent<PlayerComponent>();
}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);
}

Engine::Ref<Engine::VertexArray> BreakoutScene::GetVertexArray(std::vector<float>& vertices, std::vector<uint32_t>& indices)
{
	auto vb1 = Engine::VertexBuffer::Create(vertices.data(), vertices.size());
	vb1->SetLayout(
		{
			{ Engine::LayoutShaderType::Float3 }
		});

	auto ib = Engine::IndexBuffer::Create(indices.data(), indices.size());

	auto vao = Engine::VertexArray::Create();
	vao->SetIndexBuffer(ib);
	vao->SetVertexBuffer(vb1);
	return vao;
}

