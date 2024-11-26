#include "AttackPhase3.h"
#include <AttackPhase4.h>
#include <cmath>
#include <algorithm>
#include <random>

AttackPhase3::AttackPhase3(Boss* boss)
	: BossAttackBaseState("Phase3", boss){}


void AttackPhase3::Update()
{
    fireCounter_++;

    // 弾発射中のみボスを回転させる
    if (bulletsFired_ < 60) {
        RotateBoss();
    }

    // 3秒間（180フレーム）で弾を60発発射
    if (bulletsFired_ < 60 && fireCounter_ % 3 == 0) { // 3フレームに1回発射
        FireBullets();
        bulletsFired_++;
    }

    // 全弾発射後はクールタイム
    if (bulletsFired_ >= 60 && fireCounter_ >= 360) {
        bulletsFired_ = 0;   // 弾発射カウントをリセット
        fireCounter_ = 0;   // カウンタをリセット
    }

    // 各弾の位置を更新
    for (auto it = bullets_.begin(); it != bullets_.end();) {
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
    const float bulletSpeed = 0.5f;

    // ボスの位置を取得
    Vector3 bossPosition = boss_->GetTransform().translate;
    bossPosition.y += 5.0f; // 発射位置のy軸を5.0f上げる

    // ランダムな方向を生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    Vector3 direction = { dist(gen), dist(gen), dist(gen) };
    Normalize(direction); // ランダム方向を正規化

    BossBullet bullet(bossPosition, direction, bulletSpeed);
    bullet.Initialize();
    bullets_.push_back(std::move(bullet));
}

void AttackPhase3::RotateBoss()
{
    // ボスの回転を更新
    Transform bossTransform = boss_->GetTransform();
    bossTransform.rotate.y += 0.1f; // 毎フレーム Y軸方向に回転（値は調整可能）

    // 角度が360度を超えないようにする
    if (bossTransform.rotate.y >= 360.0f) {
        bossTransform.rotate.y -= 360.0f;
    }
    //boss_->SetTransform(bossTransform);
}

