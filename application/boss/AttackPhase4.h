#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include "Object3d.h"
#include <memory>

class AttackPhase4 : public BossAttackBaseState
{
public:
    AttackPhase4(Boss* boss);

    void Update() override;
    void Draw() override;

private:
    void DropMeteor();

    std::unique_ptr<Object3d> meteor_; // 隕石オブジェクト（1つだけ管理）
    int meteorDropCounter_ = 0;       // 隕石の降下カウント
    int coolDownCounter_ = 0;         // クールタイムのカウント
};
