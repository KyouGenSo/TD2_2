#include "LightCollision.h"
#include "Boss.h"

///=============================================================================
///						初期化
void LightCollision::Initialize() {
	//開始位置の設定
	transformStart_ = { 0.0f, 0.0f, 0.0f };

	//終了位置の設定
	transformEnd_ = { 0.0f, 0.0f, 0.0f };


	//========================================
	// ライトの位置とColliderの位置を同期
	ObjectBase::Init(transformStart_, transformEnd_, 3.0f);
}

///=============================================================================
///						更新
void LightCollision::Update() {
	ObjectBase::Update(transformStart_, transformEnd_);
}

///=============================================================================
///						当たり判定イベント
void LightCollision::OnCollision(ObjectBase* objectBase) {
	//Bossとの衝突判定
	if(dynamic_cast<Boss*>( objectBase ) != nullptr) {
		//赤色に変更
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
