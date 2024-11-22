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
    
    void FireBullets();

    std::list<BossBullet> bullets_; // 弾のリストで管理

private:
    int fireCounter_ = 0; // 弾発射用のカウンタ

};

