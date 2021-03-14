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
		-0.5, -0.5,
		0.5, -0.5,
		0.5, 0.5,
		-0.5, 0.5
	};
	
	vb = new Engine::VertexBuffer(vertices, 6*2);
	

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	ib = new Engine::IndexBuffer(indices, 6);

}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);

	renderer.Clear();

	renderer.Submit(*shader, *vb, *ib);
	
}
