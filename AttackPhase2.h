#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"

class AttackPhase2 : public BossAttackBaseState
{
public:
    AttackPhase2(Boss* boss);

    void Update() override;
    
};

