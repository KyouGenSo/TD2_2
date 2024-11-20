#include "AttackPhase2.h"

AttackPhase2::AttackPhase2(Boss* boss) : BossAttackBaseState("Phase2", boss) {}


void AttackPhase2::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 1.0f,1.0f,1.0f };
}
