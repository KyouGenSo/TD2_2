#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include "BossBullet.h"

class AttackPhase3 : public BossAttackBaseState
{
public:

    AttackPhase3(Boss* boss);

    void Update() override;

    void Draw() override;

private:

    void Normalize(Vector3& vec)
    {
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        if (length > 0.0f) {
            vec.x /= length;
            vec.y /= length;
            vec.z /= length;
        }
    }
    
    void FireBullets();

    void RotateBoss();

    std::list<BossBullet> bullets_; // 弾のリストで管理

private:
    int fireCounter_ = 0; // 弾発射用のカウンタ
    int bulletsFired_ = 0;          // 発射された弾の数を管理
};

