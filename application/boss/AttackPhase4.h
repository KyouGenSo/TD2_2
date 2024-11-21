#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"

class AttackPhase4 : public BossAttackBaseState
{
public:

    AttackPhase4(Boss* boss);

    void Update() override;

};

