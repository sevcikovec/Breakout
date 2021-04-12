#pragma once
#include <Scene/Systems.h>

class PlayerMovementSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BallSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};

class BlockSystem : public Engine::OnUpdateSystem {
	void Update(float ts) override;
};