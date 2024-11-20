#include "AttackPhase4.h"

AttackPhase4::AttackPhase4(Boss* boss) : BossAttackBaseState("Phase4", boss) {}


void AttackPhase4::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 1.0f,1.0f,1.0f };
}
