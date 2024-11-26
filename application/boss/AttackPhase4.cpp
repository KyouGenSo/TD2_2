#include "AttackPhase4.h"
#include <AttackPhase5.h>
#include <cmath>
#include <random>
#include "Player.h"

AttackPhase4::AttackPhase4(Boss* boss)
    : BossAttackBaseState("Phase4", boss), meteor_(nullptr), meteorDropCounter_(0), coolDownCounter_(0) {}

void AttackPhase4::Update()
{
    // クールタイム中はカウンタを進める
    if (coolDownCounter_ > 0) {
        coolDownCounter_--;
        return; // クールタイム中は処理を中断
    }

    // 隕石が存在しない場合、新しい隕石を生成
    if (!meteor_) {
        DropMeteor();
    }

    // 隕石の更新処理
    if (meteor_) {
        Vector3 position = meteor_->GetTranslate();
        position.y -= 1.0f; // 降下速度
        meteor_->SetTranslate(position);

        // 隕石の回転を更新
        Vector3 rotation = meteor_->GetRotate();
        rotation.y += 2.0f; // Y軸で回転
        rotation.x += 1.0f; // X軸でも回転（必要に応じて調整）
        meteor_->SetRotate(rotation);

        meteor_->Update(); // 位置と回転の変更を反映

        if (position.y < 0.0f) { // 地面に到達したら隕石を削除
            meteor_.reset();
            coolDownCounter_ = 120; // クールタイムを設定
        }
    }

    // 次のフェーズに移行
    if (boss_->GetHP() <= 200) {
        boss_->ChangeState(std::make_unique<AttackPhase5>(boss_));
    }
}

void AttackPhase4::Draw()
{
    // 隕石の描画
    if (meteor_) {
        meteor_->Draw();
    }
}

void AttackPhase4::DropMeteor()
{
    // ランダムな位置を生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-15.0f, 15.0f); // 2.0f ～ 10.0f の範囲

    float x = dist(gen);
    float z = dist(gen);
    Vector3 meteorPosition = { x, 20.0f, z }; // ランダムな位置で隕石を生成

    // 隕石オブジェクトを作成
    meteor_ = std::make_unique<Object3d>();
    meteor_->Initialize();
    meteor_->SetModel("Meteor.obj");
    meteor_->SetTranslate(meteorPosition);
    meteor_->SetScale({ 5.0f, 5.0f, 5.0f });
    meteor_->Update(); // 初期状態を反映
}
