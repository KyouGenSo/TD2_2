#define NOMINMAX
#include <cmath>
#include <algorithm> 
#include "BossNuclear.h"
#include "Boss.h"
#include "ModelManager.h"
#include "LightCollision.h"


void BossNuclear::Initialize(const Vector3& position, const Vector3& offset)
{
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Core.obj");

	transform_.translate = position;
	offset_ = offset;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Core.obj");

	alpha_ = 1.0f; // アルファ値を初期化

	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);


	//========================================
	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);
}

void BossNuclear::Update(const Vector3& bossPosition)
{
	// ボスの位置にオフセットを足して核の位置を更新
	transform_.translate = bossPosition + offset_;


	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	//========================================
	// 判定場所の処理
	Vector3 test = transform_.translate + Vector3(0.0f, 0.1f, 0.0f);
	ObjectBase::Update(transform_.translate, test);
}

void BossNuclear::Draw()
{
	if (object3d_ && alpha_ > 0.0f) {
		object3d_->SetAlpha(alpha_);
		object3d_->Draw();
	}
}

void BossNuclear::OnCollision(ObjectBase* objectBase) {
	// 衝突処理
	//Bossとの衝突判
	if (dynamic_cast<LightCollision*>(objectBase) != nullptr) {
		//赤色に変更
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	// 衝突対象がLightCollisionである場合のみ処理
	if (dynamic_cast<LightCollision*>(objectBase) != nullptr) {
		// 個別のalpha_を減少させる
		alpha_ = std::max(0.0f, alpha_ - 0.1f);

		if (alpha_ <= 0.0f) {
			// 透明になった後の処理を追加（必要に応じて）
		}	
	}

	if (alpha_ <= 0.0f && !isDestroyed_ && boss_ != nullptr) {
		isDestroyed_ = true; // 核が壊れた状態を記録
		boss_->DecreaseHP(50); // HPを減らす
	}
}
