#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include <ImGuiManager.h>

class AttackPhase1 : public BossAttackBaseState
{
public:

    AttackPhase1(Boss* boss);

    void Update() override;

};

