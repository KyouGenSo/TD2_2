/*********************************************************************
 * \file   Player.cpp
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#include "Player.h"
 ///--------------------------------------------------------------
 ///						 Engine
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include <Object3dBasic.h>
#include "Camera.h"

///=============================================================================
///						初期化
void Player::Initialize(Boss* boss) {
	//---------------------------------------
	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("Player.obj");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");

	//---------------------------------------
	// SRT
	// 大きさ
	transform_.scale = { 5.0f,5.0f,5.0f };
	// 回転
	transform_.rotate = { 0.0f,0.0f,0.0f };
	// 位置
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	boss_ = boss; // Boss のポインタを設定
}

///=============================================================================
///						更新
void Player::Update() {

	// 左右移動処理
	//const float moveSpeed = 0.6f; // 移動速度
	//if (Input::GetInstance()->PushKey(DIK_A)) {  // 左方向に移動
	//	transform_.translate.x -= moveSpeed;
	//}
	//if (Input::GetInstance()->PushKey(DIK_D)) {  // 右方向に移動
	//	transform_.translate.x += moveSpeed;
	//}

	if (boss_ == nullptr) return;

	// 左右移動の回転処理
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

	//---------------------------------------
	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	//---------------------------------------
	// 追従カメラ
	// デフォルトカメラの取得
	Camera* camera = Object3dBasic::GetInstance()->GetCamera();
	if (camera) {
		// プレイヤーの位置を中心にカメラを配置
		Vector3 cameraOffset(0.0f, 5.0f, -15.0f);
		Vector3 cameraPos = transform_.translate + cameraOffset;
		camera->SetTranslate(cameraPos);

		// カメラの向き（プレイヤーを向くように設定）
		Vector3 lookAt = transform_.translate;
		camera->SetRotate(Vector3(0.3f, 0.0f, 0.0f));
	}

}

///=============================================================================
///						描画
void Player::Draw() {
	//---------------------------------------
	// モデルの描画
	object3d_->Draw();
}
