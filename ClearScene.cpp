#include "ClearScene.h"
#include "SceneManager.h"
#include "Input.h"

void ClearScene::Initialize() {
	//clearSprite_ = std::make_unique<Sprite>("background_clear.png");
}

void ClearScene::Finalize() {

}

void ClearScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("title");
	}
}

void ClearScene::Draw() {
	//clearSprite_->SetPos(Vector2(0.0f, 0.0f));

}

void ClearScene::DrawImGui() {
	
}
