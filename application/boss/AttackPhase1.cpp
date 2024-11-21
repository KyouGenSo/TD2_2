#include "AttackPhase1.h"
#include "imgui.h"
#include <SceneManager.h>

AttackPhase1::AttackPhase1(Boss* boss) : BossAttackBaseState("Phase1",boss){}

void AttackPhase1::Update()
{
	Transform& transform = boss_->GetTransform();
	transform.scale = {2.0f,2.0f,2.0f};
}
