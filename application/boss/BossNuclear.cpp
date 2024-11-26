#include "BossNuclear.h"
#include "Boss.h"
#include "ModelManager.h"


void BossNuclear::Initialize(const Vector3& position, const Vector3& offset)
{
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Core.obj");

	transform_.translate = position;
	offset_ = offset;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Core.obj");

	//========================================
	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);
	collider_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
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
	//核の描画
	if (object3d_) {
		object3d_->Draw();
	}
}

void BossNuclear::OnCollision(ObjectBase* objectBase) {
	// 衝突処理
	//Bossとの衝突判定
	if(dynamic_cast<Boss*>( objectBase ) != nullptr) {
		//赤色に変更
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
