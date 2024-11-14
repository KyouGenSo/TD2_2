#include "ObjectBase.h"
#include "AABBCollider.h"
#include <iostream>

///====================初期化====================///
void ObjectBase::Init(const Vector3& position) {
    // コライダーの生成
	collider_ = std::make_unique<AABBCollider>(position - Vector3(0.5f, 0.5f, 0.5f), position + Vector3(0.5f, 0.5f, 0.5f)); 

    // キャラの位置とコライダーの位置を同期
    //collider_->SetTranslate(position);
}

///====================更新====================///
void ObjectBase::Update() {
    // コライダーへのトランスフォームの受け渡し
    //collider_->SetTranslate(position);
}