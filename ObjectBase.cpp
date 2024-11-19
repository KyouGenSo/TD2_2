/*********************************************************************
 * \file   ObjectBase.cpp
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#include "ObjectBase.h"
#include <iostream>
#include "ModelManager.h"

 ///====================初期化====================///
void ObjectBase::Init(Vector3& startPoint, Vector3& endPoint, float radius) {
	// コライダーの生成
	collider_ = std::make_unique<Collider>();

	// SRT
	Transform transform_;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Boss.obj");

	// スケール、回転、位置の初期設定
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	// キャラの位置とコライダーの位置を同期
	collider_->SetStart(startPoint);
	collider_->SetEnd(endPoint); // カプセルの終了座標を設定
	collider_->SetRadius(radius); // カプセルの半径を設定
}

///====================更新====================///
void ObjectBase::Update(Vector3& startPoint, Vector3& endPoint) {
	// コライダーへのトランスフォームの受け渡し
	collider_->SetStart(startPoint);
	collider_->SetEnd(endPoint); // カプセルの終了座標を更新
}

void ObjectBase::Draw() {
}
