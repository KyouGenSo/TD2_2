#include "AttackPhase3.h"

AttackPhase3::AttackPhase3(Boss* boss) : BossAttackBaseState("Phase3", boss) {}


void AttackPhase3::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 2.0f,2.0f,2.0f };
}
