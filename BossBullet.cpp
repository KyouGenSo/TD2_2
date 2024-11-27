#include "BossBullet.h"
#include "Object3d.h"
#include <ModelManager.h>
#include "LightCollision.h"

BossBullet::BossBullet(Vector3 pos, Vector3 dir, float spd)
    : position(pos), direction(dir), speed(spd), object3d_(nullptr){}

void BossBullet::Initialize()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    object3d_->SetModel("Meteor.obj"); // 弾用のモデルを設定
    object3d_->SetScale({ 1.0f, 1.0f, 1.0f }); // 弾のサイズを小さく設定

    //========================================
    // BossBulletの位置とColliderの位置を同期
    ObjectBase::Init(position, position, 1.0f);
}

void BossBullet::InitializeAsShockWave(const Vector3& pos, const Vector3& scale, const Vector3& rotate)
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    object3d_->SetModel("ShockWave.obj"); // 衝撃波用のモデルを設定
    object3d_->SetScale(scale); // 衝撃波のサイズを設定
    object3d_->SetRotate(rotate); // 衝撃波の回転を設定
    position = pos;

    //========================================
    // BossBulletの位置とColliderの位置を同期
    ObjectBase::Init(position, position, 1.0f);
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

    //========================================
    // 判定場所の処理
    Vector3 test = position + Vector3(0.0f, 1.0f, 0.0f);
    ObjectBase::Update(position, test);
}

void BossBullet::PositionUpdate()
{
    object3d_->SetTranslate(position);
    object3d_->Update();
    //========================================
    // 判定場所の処理
    Vector3 test = position + Vector3(0.0f, 1.0f, 0.0f);
    ObjectBase::Update(position, test);
}

void BossBullet::Draw() const {
    if (object3d_) {
        object3d_->Draw();
    }
}

///=============================================================================
///                     当たり判定
void BossBullet::OnCollision(ObjectBase* objectBase) {
    if(dynamic_cast<LightCollision*>( objectBase ) != nullptr) {
        collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    }
}
