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

///=============================================================================
///						初期化
void Player::Initialize() {
	//---------------------------------------
	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("Player.obj");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");

	//---------------------------------------
	// SRT
	// 大きさ
	scale_ = Vector3(1.0f, 1.0f, 1.0f);
	// 回転
	rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	// 位置
	translation_ = Vector3(0.0f, 0.0f, -13.0f);
	object3d_->SetTranslate(translation_);


}

///=============================================================================
///						更新
void Player::Update() {
	//---------------------------------------
	// モデルの更新
	object3d_->Update();

}

///=============================================================================
///						描画
void Player::Draw() {
	//---------------------------------------
	// モデルの描画
	object3d_->Draw();
}