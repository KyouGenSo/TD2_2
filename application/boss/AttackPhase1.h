#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#ifdef _DEBUG
#include <ImGuiManager.h>
#endif // DEBUG

class AttackPhase1 : public BossAttackBaseState
{
public:

    AttackPhase1(Boss* boss);

    void Update() override;

    void Draw() override;

};

