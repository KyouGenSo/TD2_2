#include "AttackPhase4.h"
#include <AttackPhase5.h>

AttackPhase4::AttackPhase4(Boss* boss) : BossAttackBaseState("Phase4", boss) {}


void AttackPhase4::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = { 1.0f,1.0f,1.0f };

	if (boss_->GetHP() <= 200) {
		boss_->ChangeState(std::make_unique<AttackPhase5>(boss_));
	}
}

void AttackPhase4::Draw()
{
}
