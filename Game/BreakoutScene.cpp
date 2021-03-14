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

	entity.AddComponent<Engine::ScriptComponent>().Bind<TestScript>();

	
	const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
	const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
	shader = new Engine::Shader(vertexShader, fragmentShader);
	
	float vertices[] = {
		-0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, -0.5,			1.0, 0.0, 0.0,
		0.5, 0.5,			0.0, 0.0, 1.0,
		-0.5, 0.5,			0.0, 0.0, 1.0
	};
	
	auto vb = Engine::VertexBuffer::Create(vertices, 5*4);
	vb->SetLayout(
		{
			{ Engine::LayoutShaderType::Float2 },
			{ Engine::LayoutShaderType::Float3 }
		});

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	auto ib = Engine::IndexBuffer::Create(indices, 6);

	va = Engine::VertexArray::Create();
	va->SetVertexBuffer(vb);
	va->SetIndexBuffer(ib);

}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);

	renderer.Clear();

	renderer.Submit(*shader, va);
	
}
