#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <memory>

class OverScene : public BaseScene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DrawImGui() override;

private:
	std::unique_ptr<Sprite> gameOverSprite_;
};
