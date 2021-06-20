#pragma once
#include <Scene/Systems.h>
#include "Physics/ACollisionListenerSystem.h"
#include <functional>
#include "../BreakoutScene.h"


class PlayerMovementSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BallSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BlockSystem : public Engine::ACollisionListenerSystem {
	void SetEntities() override;

	void Update(float ts) override;
};

class BounceSystem : public Engine::ACollisionListenerSystem {
	void SetEntities() override;

	void Update(float ts) override;
};

class OuterEdgeSystem : public Engine::ACollisionListenerSystem {
	void SetEntities() override;

	void Update(float ts) override;
};

class LightMoveSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BallSpawnerSystem : public Engine::OnUpdateSystem {
public:
	void Init(Engine::ECS* ecs) override;

	void Update(float ts) override;

	void SetShader(Engine::Ref<Engine::Shader> shader);
private:

	void CreateBall(Engine::Vec3 position, Engine::Vec3 velocity);
	
	Engine::Ref<Engine::VertexArray> ballVAO;

	Engine::Ref<Engine::Material> ballMaterial;

	float radius;
};


class ShowInfoSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class TransformAnimationSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class CameraMovementController : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class EndGameSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BlockSpawningSystem : public Engine::OnUpdateSystem {
public:
	void Update(float ts) override;

	void SetScene(BreakoutScene* scene);
private:
	BreakoutScene* scene;
};