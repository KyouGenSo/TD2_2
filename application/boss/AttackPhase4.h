#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include "Object3d.h"
#include <memory>
#include <vector>
#include "BossBullet.h"

// パーティクルのデータ構造
struct Particle {
    std::unique_ptr<Object3d> object; // 3Dオブジェクト
    Vector3 velocity;                // 速度
    int lifetime;                    // 寿命（フレーム数）
};

class AttackPhase4 : public BossAttackBaseState
{
public:
    AttackPhase4(Boss* boss);

    void Update() override;
    void Draw() override;

	BossBullet* GetMeteorCollision() { return meteorCollision_.get(); }

private:
    void DropMeteor();
    void GenerateParticles(const Vector3& position); // パーティクル生成

    std::unique_ptr<Object3d> meteor_; // 隕石オブジェクト（1つだけ管理）
    int meteorDropCounter_ = 0;       // 隕石の降下カウント
    int coolDownCounter_ = 0;         // クールタイムのカウント
    std::vector<Particle> particles_; // パーティクル管理

    //当たり判定用弾
    std::unique_ptr<BossBullet> meteorCollision_;
};
