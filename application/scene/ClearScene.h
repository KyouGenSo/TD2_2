#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <memory>

class ClearScene : public BaseScene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DrawImGui() override;

private:
	std::unique_ptr<Sprite> clearSprite_;
};
