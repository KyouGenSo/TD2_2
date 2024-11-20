#include "Boss.h"
#include "Object3d.h"
#include <cmath>
#include"math.h"
#include <Input.h>
#include "AttackPhase1.h"
#include "AttackPhase2.h"
#include "AttackPhase3.h"
#include "AttackPhase4.h"
#include "AttackPhase5.h"
#include <memory>

void Boss::Initialize() {
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Boss.obj");

	//初期位置の設定
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	ChangeState(std::make_unique<AttackPhase1>(this)); // 初期状態を設定

}

void Boss::Update() {
	// BossがPlayerの方向を向くための目標角度を計算
	Vector3 directionToPlayer = playerPosition_ - transform_.translate;
	float targetRotationY = atan2f(directionToPlayer.x, directionToPlayer.z);

	// 現在の回転角度と目標の角度の差を計算
	float angleDifference = targetRotationY - transform_.rotate.y;

	// 角度差がπ（180度）を超える場合、短い方向に回転するよう調整
	if (angleDifference > M_PI) {
		angleDifference -= 2 * static_cast<float>(M_PI);
	}
	else if (angleDifference < -M_PI) {
		angleDifference += 2 * static_cast<float>(M_PI);
	}

	// 最大回転速度を設定し、差分をゆっくり回転
	const float maxRotationSpeed = 0.02f; // 最大回転速度
	if (angleDifference > maxRotationSpeed) {
		transform_.rotate.y += maxRotationSpeed;
	}
	else if (angleDifference < -maxRotationSpeed) {
		transform_.rotate.y -= maxRotationSpeed;
	}
	else {
		// 角度差が小さい場合は目標角度に合わせる
		transform_.rotate.y = targetRotationY;
	}

	// 攻撃のステートパターン更新
	if (state_)
	{
		state_->Update();
	}

	// HPの更新
	HPUpdate();


	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Boss::Draw() {
	object3d_->Draw();
}

void Boss::DrawImGui()
{
	ImGui::Begin("phase");
	float hpRatio = static_cast<float>(hp_) / 1000.0f;
	// フェーズの変更
	if (hpRatio > 0.9f) {
		ImGui::Text("phase1");
	}
	else if (hpRatio > 0.75f ) {
		ImGui::Text("phase2");
	}
	else if (hpRatio > 0.5f ) {
		ImGui::Text("phase3");
	}
	else if (hpRatio > 0.2f) {
		ImGui::Text("phase4");
	}
	else if (hpRatio <= 0.2f) {
		ImGui::Text("phase5");
	}
	ImGui::End();
}

void Boss::HPUpdate() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		if (hp_ > 0) {
			hp_ -= 1;
			boxSize.x = static_cast<float>(hp_);
		}
	}

	float hpRatio = static_cast<float>(hp_) / 1000.0f;
	// フェーズの変更とHPバーの色を変更
	if (hpRatio > 0.9f) {
		boxColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f); // 緑
		ChangeState(std::make_unique<AttackPhase1>(this));
	}
	else if (hpRatio > 0.75f) {
		boxColor = Vector4(0.3f, 1.0f, 0.0f, 1.0f); // 黄緑
		ChangeState(std::make_unique<AttackPhase2>(this));
	}
	else if (hpRatio > 0.5f) {
		boxColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
		ChangeState(std::make_unique<AttackPhase3>(this));
	}
	else if (hpRatio > 0.2f) {
		boxColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f); // オレンジ
		ChangeState(std::make_unique<AttackPhase4>(this));
	}
	else {
		boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤
		ChangeState(std::make_unique<AttackPhase5>(this));
	}
}


void Boss::HPDraw()
{
	// 背景用の薄い黒色のボックスを描画
	Vector4 backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 0.5f); // 薄い黒色
	Vector2 backgroundSize = Vector2(1000.0f, boxSize.y);       // 初期サイズの幅と現在の高さ
	Draw2D::GetInstance()->DrawBox(boxPosition, backgroundSize, backgroundColor);

	// HPバーを描画
	Draw2D::GetInstance()->DrawBox(boxPosition, boxSize, boxColor);

	// HPバーの枠を描画
	Vector4 borderColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f); // 黒色

	// 枠の4辺を描画
	Vector2 topLeft = boxPosition;
	Vector2 topRight = Vector2(boxPosition.x + 1000.0f, boxPosition.y); // HP最大値に基づく固定幅
	Vector2 bottomLeft = Vector2(boxPosition.x, boxPosition.y + boxSize.y);
	Vector2 bottomRight = Vector2(boxPosition.x + 1000.0f, boxPosition.y + boxSize.y);

	Draw2D::GetInstance()->DrawLine(topLeft, topRight, borderColor);      // 上辺
	Draw2D::GetInstance()->DrawLine(topRight, bottomRight, borderColor);  // 右辺
	Draw2D::GetInstance()->DrawLine(bottomRight, bottomLeft, borderColor); // 下辺
	Draw2D::GetInstance()->DrawLine(bottomLeft, topLeft, borderColor);    // 左辺
}

void Boss::ChangeState(std::unique_ptr<BossAttackBaseState> state)
{
	state_ = std::move(state);
}

