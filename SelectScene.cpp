#include "SelectScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Vector2.h"

void SelectScene::Initialize() {
	//selectSprite_ = std::make_unique<Sprite>("background_select.png");

}

void SelectScene::Finalize() {

}

void SelectScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("game");  // ゲーム画面に遷移
	}
}

void SelectScene::Draw() {
	//selectSprite_->SetPos(Vector2(0.0f, 0.0f));

}

void SelectScene::DrawImGui() {

}
