#include <Scene\Entity.cpp>
#include "BreakoutScene.h"
#include <cstdio>
#include "Scripts/TestScript.cpp"
#include <Scene\Components.h>

BreakoutScene::BreakoutScene() : Engine::Scene("Main scene")
{
	// initialize scene
	
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
	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	auto shader = Engine::CreateRef<Engine::Shader>(vertexShader, fragmentShader);
	
	float vertices1[] = {
		-0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, 0.5,			0.0, 0.0, 1.0,
		-0.5, 0.5,			0.0, 0.0, 1.0
	};
	
	auto vb1 = Engine::VertexBuffer::Create(vertices1, 5 * 4);
	vb1->SetLayout(
		{
			{ Engine::LayoutShaderType::Float2 },
			{ Engine::LayoutShaderType::Float3 }
		});

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	auto ib = Engine::IndexBuffer::Create(indices, 6);

	auto vao = Engine::VertexArray::Create();
	vao->SetIndexBuffer(ib);
	vao->SetVertexBuffer(vb1);

	Engine::Entity entity = CreateEntity("Test entity");
	auto& transform = entity.AddComponent<Engine::TransformComponent>();

	auto& mesh = entity.AddComponent<Engine::MeshComponent>();

	mesh.shader = shader;
	mesh.vao = vao;
}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);
}

