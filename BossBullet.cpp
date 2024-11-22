#include "BossBullet.h"
#include "Object3d.h"
#include <ModelManager.h>

BossBullet::BossBullet(Vector3 pos, Vector3 dir, float spd)
	: position(pos), direction(dir), speed(spd), object3d_(nullptr){}

void BossBullet::Initialize()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    object3d_->SetModel("player.obj"); // 弾用のモデルを設定
    object3d_->SetScale({ 1.0f, 1.0f, 1.0f }); // 弾のサイズを小さく設定
}

void BossBullet::Update()
{
    // 位置を更新
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    // Object3dの位置も更新
    object3d_->SetTranslate(position);
    object3d_->Update();
}

void BossBullet::Draw() const
{
    if (object3d_) {
        object3d_->Draw();
    }
}
