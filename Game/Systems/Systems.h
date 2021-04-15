#pragma once
#include <Scene/Systems.h>
#include "Physics/ACollisionListenerSystem.h"


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