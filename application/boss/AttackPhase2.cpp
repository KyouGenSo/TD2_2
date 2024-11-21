#include "AttackPhase2.h"
#include <AttackPhase3.h>

AttackPhase2::AttackPhase2(Boss* boss) : BossAttackBaseState("Phase2", boss) {}


void AttackPhase2::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 1.0f,1.0f,1.0f };

	// フェーズ終了条件: HPが50%以下
	if (boss_->GetHP() <= 750) {
		boss_->ChangeState(std::make_unique<AttackPhase3>(boss_));
	}
}

void AttackPhase2::Draw()
{
}
