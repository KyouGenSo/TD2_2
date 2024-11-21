#include "AttackPhase3.h"
#include <AttackPhase4.h>

AttackPhase3::AttackPhase3(Boss* boss)
	: BossAttackBaseState("Phase3", boss){}


void AttackPhase3::Update()
{
    Transform& transform = boss_->GetTransform();

    // ボスのサイズを変更（デバッグ用）
    transform.scale = { 2.0f, 2.0f, 2.0f };

    // フェーズ終了条件: HPが50%以下
    if (boss_->GetHP() <= 500) {
        boss_->ChangeState(std::make_unique<AttackPhase4>(boss_));
    }
}

void AttackPhase3::Draw()
{
   
}
