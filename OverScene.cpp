#include "OverScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "SpriteBasic.h"

void OverScene::Initialize() {
	// Game Over 表示用のスプライトを初期化
	//gameOverSprite_ = std::make_unique<Sprite>("background_gameover.png");
}

void OverScene::Finalize() {
	// 必要な後処理（現在は何もなし）
}

void OverScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("title"); // タイトル画面に戻る
	}
}

void OverScene::Draw() {
	// Game Over スプライトを描画
	//gameOverSprite_->SetPos(Vector2(0.0f, 0.0f));
	//gameOverSprite_->Draw();
}

void OverScene::DrawImGui() {
	// 必要ならデバッグ情報を表示（現在は何もなし）
}
