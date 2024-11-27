#pragma once
#include <Vector3.h>
#include <memory>
#include "ObjectBase.h"

class Object3d;

class BossBullet : public ObjectBase
{
public:
    Vector3 position;  // 弾の現在位置
    Vector3 direction; // 弾の進行方向
    float speed;       // 弾の速度

    // コンストラクタ：弾の初期状態を設定
    BossBullet(Vector3 pos, Vector3 dir, float spd);

    void Initialize();
    void InitializeAsShockWave(const Vector3& pos, const Vector3& scale, const Vector3& rotate);
    void Update();
    void PositionUpdate();
    void Draw() const;
	void SetRadius(float radius) { collider_->SetRadius(radius); }

    void OnCollision(ObjectBase* objectBase) override;

private:
    std::unique_ptr<Object3d> object3d_; // 弾の3Dオブジェクト
};
