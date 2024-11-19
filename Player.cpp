#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Camera.h"
#include "ImGuiManager.h"
#include <cmath>
#include <numbers>

void Player::Initialize(Boss* boss) {
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Player.obj");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");

	// スケール、回転、位置の初期設定
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	boss_ = boss; // Boss のポインタを設定

	followCamera_ = std::make_unique<FollowCamera>();


	// lightの初期設定
	lightPos_ = { transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z };
	lightDir_ = boss_->GetTransform().translate - lightPos_;
	lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightIntensity_ = 1.0f;
	lightRange_ = 10.0f;
	lightDecay_ = 1.0f;
	lightSpotAngle_ = std::cos(std::numbers::pi_v<float> / 3.0f);
	isSpotLight_ = true;

}

void Player::Update() {

	// Boss が存在しない場合、処理をスキップ
	//（Boss の位置に依存した処理があるため、nullptr の場合にクラッシュを防ぐための安全対策）
	if (boss_ == nullptr) return;

	// 移動処理
	Move();

	// BossにPlayerの位置を通知
	boss_->SetPlayerPosition(transform_.translate);

	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	// ライト
	lightPos_ = { transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z };
	lightDir_ = boss_->GetTransform().translate - lightPos_;
	Object3dBasic::GetInstance()->SetSpotLight(lightPos_, lightDir_.normalize(), lightColor_, lightIntensity_, lightRange_, lightDecay_, lightSpotAngle_, isSpotLight_);

	// 追従カメラ
	followCamera_->Update(transform_.translate, transform_.rotate);

}

void Player::Draw() {
	// モデルの描画
	object3d_->Draw();
}

void Player::Move()
{
	// プレイヤーの左右移動 (Boss の周りを回転)
	if (Input::GetInstance()->PushKey(DIK_A)) {
		angle_ -= rotationSpeed_; // 左回転
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		angle_ += rotationSpeed_; // 右回転
	}

	// Boss の位置を中心に円状に移動
	const Transform& bossTransform = boss_->GetTransform();
	transform_.translate.x = bossTransform.translate.x + radius_ * cosf(angle_);
	transform_.translate.z = bossTransform.translate.z + radius_ * sinf(angle_);

	// Boss の方向を向くための角度計算
	Vector3 directionToBoss = bossTransform.translate - transform_.translate;
	transform_.rotate.y = atan2f(directionToBoss.x, directionToBoss.z);

	// ジャンプ処理
	if (Input::GetInstance()->PushKey(DIK_W) && !isJumping_) {
		isJumping_ = true;
		jumpVelocity_ = jumpPower_;
	}

	if (isJumping_) {
		transform_.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		if (transform_.translate.y <= 0.0f) {
			transform_.translate.y = 0.0f;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
		}
	}
}

void Player::DrawImGui()
{
	ImGui::Begin("Player SpotLight");
	ImGui::DragFloat3("Light Pos", &lightPos_.x, 0.1f);
	ImGui::DragFloat3("Light Dir", &lightDir_.x, 0.1f);
	ImGui::ColorEdit4("Light Color", &lightColor_.x);
	ImGui::SliderFloat("Light Intensity", &lightIntensity_, 0.0f, 10.0f);
	ImGui::SliderFloat("Light Range", &lightRange_, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Decay", &lightDecay_, 0.0f, 2.0f);
	ImGui::SliderFloat("Light Spot Angle", &lightSpotAngle_, 0.0f, 1.0f);
	ImGui::Checkbox("SpotLight", &isSpotLight_);
	ImGui::End();

}
