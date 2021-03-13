#include <Scene\Entity.cpp>
#include "BreakoutScene.h"
#include <cstdio>
#include "Scripts/TestScript.cpp"
#include <Scene\Components.h>
BreakoutScene::BreakoutScene() : Engine::Scene("Main scene")
{
	// initialize scene
	Engine::Entity entity = CreateEntity("Test entity");

	
	std::string test = "test string";
	for (int i = 0; i < 100; i++) {
		CreateEntity(test + std::to_string(i));
	}

	entity.AddComponent<Engine::ScriptComponent>().Bind<TestScript>();
}

void BreakoutScene::OnUpdate(float frameTimeMS)
{
	Engine::Scene::OnUpdate(frameTimeMS);
	
}
