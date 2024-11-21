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



	//// lightの初期設定-----------------------------------------------------------------------------------------------------------------------------

	// 範囲が狭く光が強いライト設定
	narrowStrongLight_ = {
		{ transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z }, // lightPos
		boss->GetTransform().translate - Vector3(transform_.translate.x, transform_.translate.y + 5.0f, transform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		10.0f, // 光の強さ
		26.0f, // ライト範囲
		0.1f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 20.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 範囲が広く光が弱いライト設定
	wideWeakLight_ = {
		{ transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z }, // lightPos
		boss->GetTransform().translate - Vector3(transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		10.0f, // 光の強さ
		50.0f, // ライト範囲
		1.0f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 5.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 初期ライト設定
	currentLight_ = &narrowStrongLight_;

	// ライトの位置と方向を常に更新
	//currentLight_->lightPos = { transform_.translate.x, currentLight_->lightPos.y, transform_.translate.z };
	//currentLight_->lightDir = (boss_->GetTransform().translate - currentLight_->lightPos).normalize();

	////-------------------------------------------------------------------------------------------------------------------------------------------


	actionDelay_ = 180; // 初期の制限時間を設定
	canAct_ = false;   // 初期状態では行動不可

}

void Player::Update() {
	// ボスが存在しない場合、処理をスキップ
	if (boss_ == nullptr) return;

	//ライト
	Light();

	// 移動処理
	Move();

	// BossにPlayerの位置を通知
	boss_->SetPlayerPosition(transform_.translate);

	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

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

void Player::Light()
{
	// ライト切り替え
	if (Input::GetInstance()->TriggerKey(DIK_L)) {
		if (isLightProfileToggled_) {
			currentLight_ = &narrowStrongLight_;
		} else {
			currentLight_ = &wideWeakLight_;
		}
		isLightProfileToggled_ = !isLightProfileToggled_;
	}

	// F3キーでライトを下に、F4キーでライトを上に移動
	if (Input::GetInstance()->PushKey(DIK_F3)) {
		currentLight_->lightDir.y -= 0.1f; // 下方向に移動
	}
	if (Input::GetInstance()->PushKey(DIK_F4)) {
		currentLight_->lightDir.y += 0.1f; // 上方向に移動
	}

	// ライトの位置と方向を常に更新
	currentLight_->lightPos = { transform_.translate.x, currentLight_->lightPos.y, transform_.translate.z };
	currentLight_->lightDir = (boss_->GetTransform().translate - currentLight_->lightPos).normalize();

	// スポットライトの更新
	Object3dBasic::GetInstance()->SetSpotLight(
		currentLight_->lightPos,
		currentLight_->lightDir.normalize(), // 正規化
		currentLight_->lightColor,
		currentLight_->lightIntensity,
		currentLight_->lightRange,
		currentLight_->lightDecay,
		currentLight_->lightSpotAngle,
		currentLight_->isSpotLight
	);
}

void Player::DrawImGui()
{
	ImGui::Begin("Player SpotLight");

	ImGui::Text("Current Light Profile: %s", isLightProfileToggled_ ? "Wide Weak" : "Narrow Strong");
	ImGui::DragFloat3("Light Dir", &currentLight_->lightDir.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("Light Pos", &currentLight_->lightPos.x, 0.1f);
	ImGui::ColorEdit4("Light Color", &currentLight_->lightColor.x);
	ImGui::SliderFloat("Light Intensity", &currentLight_->lightIntensity, 0.0f, 10.0f);
	ImGui::SliderFloat("Light Range", &currentLight_->lightRange, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Decay", &currentLight_->lightDecay, 0.0f, 2.0f);
	ImGui::SliderFloat("Light Spot Angle", &currentLight_->lightSpotAngle, 0.0f, 1.0f);
	ImGui::Checkbox("SpotLight", &currentLight_->isSpotLight);

	ImGui::End();
}

