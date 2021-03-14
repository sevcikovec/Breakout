#include <Scene\Entity.cpp>
#include "BreakoutScene.h"
#include <cstdio>
#include "Scripts/TestScript.cpp"
#include <Scene\Components.h>
BreakoutScene::BreakoutScene() : Engine::Scene("Main scene")
{
	// initialize scene
	Engine::Entity entity = CreateEntity("Test entity");

	/*
	std::string test = "test string";
	for (int i = 0; i < 100; i++) {
		CreateEntity(test + std::to_string(i));
	}*/

	auto& scriptComponent = entity.AddComponent<Engine::ScriptComponent>();
	scriptComponent.Bind<TestScript>();
	scriptComponent.InstantiateScript(&scriptComponent);

	auto testScript = static_cast<TestScript*>(scriptComponent.Instance);
	testScript->OnCreate();
	
	testScript->OnStateChanged = [this]() {
		std::cout << " Callback on state changed" << std::endl;
		SwapVAs();
	};

	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	shader = new Engine::Shader(vertexShader, fragmentShader);
	
	float vertices1[] = {
		-0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, 0.5,			0.0, 0.0, 1.0,
		-0.5, 0.5,			0.0, 0.0, 1.0
	};

	float vertices2[] = {
		-0.5, -0.5,			0.0, 0.0, 1.0,
		0.5, -0.5,			0.0, 0.0, 1.0,
		0.5, 0.5,			1.0, 0.0, 0.0,
		-0.5, 0.5,			1.0, 0.0, 0.0
	};
	
	auto vb1 = Engine::VertexBuffer::Create(vertices1, 5 * 4);
	vb1->SetLayout(
		{
			{ Engine::LayoutShaderType::Float2 },
			{ Engine::LayoutShaderType::Float3 }
		});

	auto vb2 = Engine::VertexBuffer::Create(vertices2, 5 * 4);
	vb2->SetLayout(
		{
			{ Engine::LayoutShaderType::Float2 },
			{ Engine::LayoutShaderType::Float3 }
		});

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	auto ib = Engine::IndexBuffer::Create(indices, 6);

	firstVA = Engine::VertexArray::Create();
	firstVA->SetVertexBuffer(vb1);
	firstVA->SetIndexBuffer(ib);

	secondVA = Engine::VertexArray::Create();
	secondVA->SetVertexBuffer(vb2);
	secondVA->SetIndexBuffer(ib);

}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);

	renderer.Clear();

	if (firstActive) renderer.Submit(*shader, firstVA);
	else renderer.Submit(*shader, secondVA);
	
}

void BreakoutScene::SwapVAs()
{
	firstActive = !firstActive;
}
