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
	if (isDestroyed_) {
		// 破壊された場合、タイマーを進める
		destructionTimer_ += 1.0f / 60.0f;
		if (destructionTimer_ >= destructionDuration_) {
			SetVisible(false); // 一定時間後に非表示
		}
		return; // 破壊中は位置を更新しない
	}

	// 通常の処理
	transform_.translate = bossPosition + offset_;
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	// 判定場所の処理
	Vector3 test = transform_.translate + Vector3(0.0f, 0.1f, 0.0f);
	ObjectBase::Update(transform_.translate, test);
}

void BossNuclear::Draw()
{
	if (object3d_ && alpha_ > 0.0f && isVisible_) {
		object3d_->SetAlpha(alpha_);
		object3d_->Draw();
	}
}

void BossNuclear::OnCollision(ObjectBase* objectBase) {
	if (!isVisible_ || isDestroyed_) return; // 可視状態でない、または既に破壊済みの場合は無視

	// 通常の衝突処理
	if (dynamic_cast<LightCollision*>(objectBase) != nullptr) {
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		alpha_ = std::max(0.0f, alpha_ - 0.1f);

		if (alpha_ <= 0.0f) {
			isDestroyed_ = true; // 破壊フラグを立てる
			destructionTimer_ = 0.0f; // タイマー初期化
			boss_->DecreaseHP(50); // ボスのHPを減少
		}
	}
}

void BossNuclear::ResetCore()
{
	isDestroyed_ = false;       // 破壊フラグをリセット
	destructionTimer_ = 0.0f;   // タイマーをリセット
	alpha_ = 1.0f;              // アルファ値をリセット
	SetVisible(true);           // コアを表示
}
