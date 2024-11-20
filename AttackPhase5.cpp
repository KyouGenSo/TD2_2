#include "AttackPhase5.h"

AttackPhase5::AttackPhase5(Boss* boss) : BossAttackBaseState("Phase5", boss) {}


void AttackPhase5::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 2.0f,2.0f,2.0f };
}
