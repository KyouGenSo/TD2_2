#pragma once
#include <Vector3.h>
#include <memory>

class Object3d;

class BossBullet
{
public:
    Vector3 position;  // 弾の現在位置
    Vector3 direction; // 弾の進行方向
    float speed;       // 弾の速度

    // コンストラクタ：弾の初期状態を設定
    BossBullet(Vector3 pos, Vector3 dir, float spd);

    void Initialize();
    void Update();
    void Draw() const;

private:
    std::unique_ptr<Object3d> object3d_; // 弾の3Dオブジェクト

};

