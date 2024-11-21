#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"

class AttackPhase3 : public BossAttackBaseState
{
public:

    AttackPhase3(Boss* boss);

    void Update() override;

};

