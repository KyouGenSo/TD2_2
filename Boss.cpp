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
	

	// 状態に応じた処理を実行
	// 現在のフェーズに対応する関数を呼び出す
	(this->*spFuncTable[static_cast<int>(phase_)])();

	// HPの更新
	HPUpdate();


	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Boss::Move()
{
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
}

void Boss::AttackPhase()
{
	// アタックフェーズの変更
	float hpRatio = static_cast<float>(hp_) / 1000.0f;
	if (hpRatio > 0.9f) {
		ChangeState(std::make_unique<AttackPhase1>(this));
	}
	else if (hpRatio > 0.75f) {
		ChangeState(std::make_unique<AttackPhase2>(this));
	}
	else if (hpRatio > 0.5f) {
		ChangeState(std::make_unique<AttackPhase3>(this));
	}
	else if (hpRatio > 0.2f) {
		ChangeState(std::make_unique<AttackPhase4>(this));
	}
	else {
		ChangeState(std::make_unique<AttackPhase5>(this));
	}
}

void Boss::Draw() {
	object3d_->Draw();
}

void Boss::DrawImGui()
{
	ImGui::Begin("phase");
	// フェーズの変更
	float hpRatio = static_cast<float>(hp_) / 1000.0f;
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

	// HPバーの色を変更
	float hpRatio = static_cast<float>(hp_) / 1000.0f;
	if (hpRatio > 0.9f) {
		boxColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f); // 緑
	}
	else if (hpRatio > 0.75f) {
		boxColor = Vector4(0.3f, 1.0f, 0.0f, 1.0f); // 黄緑
	}
	else if (hpRatio > 0.5f) {
		boxColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
	}
	else if (hpRatio > 0.2f) {
		boxColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f); // オレンジ
	}
	else {
		boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤
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

// メンバ関数ポインタのテーブルの実体
void (Boss::* Boss::spFuncTable[])() = {
	&Boss::Usually,
	&Boss::Down,
	& Boss::GettingUp
};

void Boss::Usually()
{
	// 通常の動き
	Move();

	// ステートの更新
	if (state_) {
		state_->Update();
	}

	// アタックのフェーズ変更
	AttackPhase();

	if (Input::GetInstance()->PushKey(DIK_V))
	{
		phase_ = Phase::Down;
	}
}

void Boss::Down() {
	// イージング用のタイマー
	static float easingTime = 0.0f;      // 現在の時間
	static int waitTimer = 0;            // バウンド後の待機タイマー
	const float fallDuration = 2.0f;    // 倒れるまでの時間（秒）
	const float bounceDuration = 1.0f;  // バウンドの時間（秒）

	const float maxRotation = static_cast<float>(M_PI) / 2.0f; // 最大回転角（90度）

	// イージングタイマーの進行
	if (easingTime < fallDuration + bounceDuration) {
		easingTime += 0.02f; // 時間を進める（フレーム単位で調整）
	}

	if (easingTime <= fallDuration) {
		// 正規化時間 t を計算（倒れる動きの進行）
		float t = easingTime / fallDuration;
		if (t > 1.0f) t = 1.0f;

		// イージング値を取得（倒れる動き）
		float easedT = EaseInExpo(t);

		// 回転角の更新
		transform_.rotate.x = maxRotation * easedT;
	}
	else if (easingTime <= fallDuration + bounceDuration) {
		// 正規化時間 t を計算（バウンドの進行）
		float t = (easingTime - fallDuration) / bounceDuration;
		if (t > 1.0f) t = 1.0f;

		// イージング値を取得（バウンド）
		float easedT = EaseOutBounce(t);

		// 回転角の更新（バウンド）
		transform_.rotate.x = maxRotation - (maxRotation * 0.1f * easedT); // 最大回転角から少し戻る動き
	}
	else {
		// バウンドが終了後の待機
		waitTimer++;
		if (waitTimer >= 360) {
			// 待機時間が経過したら起き上がりフェーズに移行
			easingTime = 0.0f;
			waitTimer = 0;
			phase_ = Phase::GettingUp;
		}
	}

	// リセット（デバッグ用）
	if (Input::GetInstance()->PushKey(DIK_B)) {
		transform_.rotate.x = 0.0f; // 回転をリセット
		easingTime = 0.0f;          // タイマーをリセット
		waitTimer = 0;              // 待機タイマーをリセット
		phase_ = Phase::Usually;    // 通常状態に戻す
	}
}


void Boss::GettingUp() {
	// イージング用のタイマー
	static float easingTime = 0.0f; // 起き上がる時間
	const float getUpDuration = 10.0f; // 起き上がりにかける時間（秒）

	// イージングタイマーの進行
	if (easingTime < getUpDuration) {
		easingTime += 0.02f; // 時間を進める
	}

	// 正規化時間 t を計算（0.0f～1.0f）
	float t = easingTime / getUpDuration;
	if (t > 1.0f) t = 1.0f;

	// イージング値を取得（起き上がる動き）
	float easedT = EaseInOut(t);

	// 回転角の更新（ゆっくり元に戻す）
	transform_.rotate.x = (1.0f - easedT) * (static_cast<float>(M_PI) / 2.0f);

	// 位置の更新（元の位置に戻す）
	transform_.translate.x *= (1.0f - easedT);

	// 起き上がりが完了したら通常状態に戻す
	if (t >= 1.0f) {
		transform_.rotate.x = 0.0f;  // 回転を完全にリセット
		transform_.translate.x = 0.0f; // 位置を完全にリセット
		easingTime = 0.0f;           // タイマーをリセット
		phase_ = Phase::Usually;     // 通常状態に戻す
	}
}




