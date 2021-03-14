#pragma once
#include "Scene/Scene.h"
#include <Renderer\Shader.h>
#include <Renderer\VertexBuffer.h>
#include <Renderer\Renderer.h>


class BreakoutScene : public Engine::Scene {
public:
	BreakoutScene();

	void OnUpdate(float frameTimeMS);

	void SwapVAs();

private:
	Engine::Shader* shader;

	Engine::Ref<Engine::VertexArray> firstVA;

	Engine::Ref<Engine::VertexArray> secondVA;

	bool firstActive;

	Engine::Renderer renderer;
};