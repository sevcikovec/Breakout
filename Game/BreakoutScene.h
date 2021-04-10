#pragma once
#include "Scene/Scene.h"
#include "Utils.h"
#include "Renderer/Material.h"
#include "Renderer/VertexArray.h"


class BreakoutScene : public Engine::Scene {
public:
	BreakoutScene();

	void OnUpdate(float frameTimeMS);

private:
	Engine::Ref<Engine::VertexArray> GetVertexArray(std::vector<float>& vertices, std::vector<uint32_t>& indices);

	void CreatePlayerArch(Engine::Ref<Engine::Material> playerArchMaterial, Engine::Ref<Engine::VertexArray> playerMeshVAO, float startingAngle, float radius);
};