#pragma once
#include "Scene/Scene.h"
#include <Renderer\Shader.h>
#include <Renderer\VertexBuffer.h>
#include <Renderer\Renderer.h>


class BreakoutScene : public Engine::Scene {
public:
	BreakoutScene();

	void OnUpdate(float frameTimeMS);

private:
	Engine::Shader* shader;

	Engine::Ref<Engine::VertexArray> va;

	Engine::Renderer renderer;
};