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

	// コライダーの初期化
	collider_->Initialize();

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
	// コライダーの更新
	collider_->DrawCapsule();
}
