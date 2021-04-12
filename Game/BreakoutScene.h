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

	Engine::Entity CreatePlayerArch(Engine::Ref<Engine::Material> material, Engine::Ref<Engine::VertexArray> vertexArray, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, Engine::AABB_local aabb);
	
	Engine::Entity CreateArch(Engine::Ref<Engine::Material> material, Engine::Ref<Engine::VertexArray> vertexArray, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, float yPos, Engine::AABB_local aabb);

	Engine::Entity CreateBlockArch(Engine::Ref<Engine::Material> material, Engine::Ref<Engine::VertexArray> vertexArray, float angleWidth, float startingAngle, float radius, float innerRadius, float outerRadius, float yPos, Engine::AABB_local aabb);

};