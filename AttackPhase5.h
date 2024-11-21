#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"

class AttackPhase5 : public BossAttackBaseState
{
public:

    AttackPhase5(Boss* boss);
   
    void Update() override;
};

