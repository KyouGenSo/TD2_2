#include "BossNuclear.h"

void BossNuclear::Initialize(const Vector3& position, const Vector3& offset)
{
	transform_.translate = position;
	offset_ = offset;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Nuclear.obj");
}

void BossNuclear::Update(const Vector3& bossPosition)
{
	// ボスの位置にオフセットを足して核の位置を更新
	transform_.translate = bossPosition + offset_;


	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();
}

void BossNuclear::Draw()
{
	// 核の描画
	if (object3d_) {
		object3d_->Draw();
	}
}
