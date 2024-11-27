#include "AttackPhase4.h"
#include <AttackPhase5.h>
#include <cmath>
#include <random>
#include "Player.h"

AttackPhase4::AttackPhase4(Boss* boss)
    : BossAttackBaseState("Phase4", boss), meteor_(nullptr), meteorDropCounter_(0), coolDownCounter_(0) {}

void AttackPhase4::GenerateParticles(const Vector3& position)
{
    // ランダムな速度を生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distZ(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distY(0.8f, 1.2f); // Y方向の速度

    for (int i = 0; i < 10; ++i) {
        // パーティクルオブジェクトの初期化
        auto particleObject = std::make_unique<Object3d>();
        particleObject->Initialize();
        particleObject->SetModel("Meteor.obj");
        particleObject->SetTranslate(position);
        particleObject->SetScale({ 0.4f, 0.4f, 0.4f });
        particleObject->Update();

        // パーティクルデータを追加
        Vector3 velocity = { distX(gen), distY(gen), distZ(gen) };
        Particle particle = { std::move(particleObject), velocity, 180 }; // 寿命を100フレームに設定
        particles_.emplace_back(std::move(particle));
    }
;
}

void AttackPhase4::Update()
{
    // パーティクルの更新
    for (auto it = particles_.begin(); it != particles_.end(); ) {
        Particle& particle = *it;

        // パーティクルの位置を更新
        Vector3 position = particle.object->GetTranslate();
        position.x += particle.velocity.x;
        position.y += particle.velocity.y;
        position.z += particle.velocity.z;

        // 空気抵抗を追加して徐々に速度を減少
        /*particle.velocity.x *= 0.98f;
        particle.velocity.y *= 0.98f;
        particle.velocity.z *= 0.98f;*/

        // 簡易的な重力処理
        particle.velocity.y -= 0.1f;
        particle.object->SetTranslate(position);
        particle.object->Update();

        // 寿命を減らし、寿命切れまたは地面に到達したら削除
        particle.lifetime--;
        if (particle.lifetime <= 0 || position.y < 0.0f) {
            it = particles_.erase(it);
			meteorCollision_->SetRadius(0.01f);
        }
        else {
            ++it;
        }
    }

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
		// NOTE: 隕石の衝突判定用オブジェクトの位置を更新
		meteorCollision_->position = meteor_->GetTranslate();
		meteorCollision_->PositionUpdate();

        // 隕石の回転を更新
        Vector3 rotation = meteor_->GetRotate();
        rotation.y += 2.0f; // Y軸で回転
        rotation.x += 1.0f; // X軸でも回転（必要に応じて調整）
        meteor_->SetRotate(rotation);

        meteor_->Update(); // 位置と回転の変更を反映

        if (position.y < 0.0f) { // 地面に到達したら隕石を削除
            GenerateParticles(position);
            meteor_.reset();
            coolDownCounter_ = 120; // クールタイムを設定
        }
    }

    // フェーズ遷移の条件
    if (boss_->GetHP() <= 30) {
        boss_->ChangeState(std::make_unique<AttackPhase5>(boss_));
    }
}

void AttackPhase4::Draw()
{
    // 隕石の描画
    if (meteor_) {
        meteor_->Draw();
    }

    // パーティクルの描画
    for (const auto& particle : particles_) {
        particle.object->Draw();
    }
}

void AttackPhase4::DropMeteor()
{
    // ランダムな位置を生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-15.0f, 15.0f);

    float x = dist(gen);
    float z = dist(gen);
    Vector3 meteorPosition = { x, 20.0f, z };

    // 隕石オブジェクトを初期化
    meteor_ = std::make_unique<Object3d>();
    meteor_->Initialize();
    meteor_->SetModel("Meteor.obj");
    meteor_->SetTranslate(meteorPosition);
    meteor_->SetScale({ 5.0f, 5.0f, 5.0f });
    meteor_->Update();

    // 隕石の衝突判定用オブジェクトを生成
    // NOTE:
    meteorCollision_ = std::make_unique<BossBullet>(meteor_->GetTranslate() , Vector3(0.0f, 0.0f, 0.0f), 0.0f);
    meteorCollision_->InitializeAsShockWave(meteor_->GetTranslate(), Vector3(5.0f, 5.0f, 5.0f), Vector3(0.0f, 0.0f, 0.0f));
	meteorCollision_->SetRadius(13.0f);
}
