#include "AttackPhase3.h"
#include <AttackPhase4.h>
#include <cmath>
#include <algorithm>

AttackPhase3::AttackPhase3(Boss* boss)
	: BossAttackBaseState("Phase3", boss){}


void AttackPhase3::Update()
{
    fireCounter_++;

    // フレームごとに弾を発射
    if (fireCounter_ >= 180) {
        FireBullets();
        fireCounter_ = 0; // カウンタをリセット
    }


    // 各弾の位置を更新
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        it->Update();

        // 弾が画面外に出たら削除
        if (it->position.x < -50.0f || it->position.x > 50.0f ||
            it->position.y < -50.0f || it->position.y > 50.0f ||
            it->position.z < -50.0f || it->position.z > 50.0f) {
            it = bullets_.erase(it);
        }
        else {
            ++it;
        }
    }

    // フェーズ終了条件: HPが50%以下
    if (boss_->GetHP() <= 500) {
        boss_->ChangeState(std::make_unique<AttackPhase4>(boss_));
    }
}

void AttackPhase3::Draw()
{
    for (const auto& bullet : bullets_) {
        bullet.Draw();  // 各弾を描画
    }
}

void AttackPhase3::FireBullets() {
    const int bulletCount = 12;
    const float angleStep = 2.0f * static_cast<float>(M_PI) / bulletCount;
    const float bulletSpeed = 0.5f;

    // ボスの位置を取得
    Vector3 bossPosition = boss_->GetTransform().translate;

    for (int i = 0; i < bulletCount; ++i) {
        float angle = i * angleStep;
        Vector3 direction = { std::cos(angle), 0.0f, std::sin(angle) };
        BossBullet bullet(bossPosition, direction, bulletSpeed);
        bullet.Initialize();
        bullets_.push_back(std::move(bullet));
    }
}

