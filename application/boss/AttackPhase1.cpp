#include "AttackPhase1.h"
#include "imgui.h"
#include <SceneManager.h>
#include <AttackPhase2.h>

AttackPhase1::AttackPhase1(Boss* boss) : BossAttackBaseState("Phase1",boss){}

void AttackPhase1::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = {2.0f,2.0f,2.0f};

    // フェーズ終了条件: HPが90%以下
    if (boss_->GetHP() <= 900) {
        boss_->ChangeState(std::make_unique<AttackPhase2>(boss_));
    }
}

void AttackPhase1::Draw()
{
}
