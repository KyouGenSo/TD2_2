#include "AttackPhase2.h"
#include <cmath>
constexpr float kPI = 3.14159265358979323846f;
#include <random>
#include <AttackPhase3.h>
#include "FollowCamera.h"

AttackPhase2::AttackPhase2(Boss* boss) : BossAttackBaseState("Phase2", boss), followCamera_(std::make_unique<FollowCamera>()) {
    InitializeShockWaveObjects(shockWaveObjectCount_);
}

void AttackPhase2::Update() {
    Transform& transform = boss_->GetTransform();

    switch (state_) {
    case PhaseState::Ascending:
        transform.translate.y += ascendingSpeed_;
        if (transform.translate.y >= 7.0f) {
            state_ = PhaseState::Descending;
        }
        break;

    case PhaseState::Descending:
        transform.translate.y -= descendingSpeed_;
        if (transform.translate.y <= 0.0f) {
            transform.translate.y = 0.0f;
            state_ = PhaseState::ShockWave;
            ActivateShockWave();

            //followCamera_ = std::make_unique<FollowCamera>();
            followCamera_->StartShake(80, 10.5f);
            //followCamera_->UpdateShake();
            
        }
        break;

    case PhaseState::ShockWave:
        if (shockWaveActive_) {
            UpdateShockWaveObjects();
        }
        else {
            state_ = PhaseState::Reset;
        }
        break;

    case PhaseState::Reset:
        state_ = PhaseState::Ascending; // 次の上昇に戻る
        break;
    }

    if (followCamera_) {
        followCamera_->UpdateShake();
    }

    // フェーズ終了条件: y軸が0.0fでHPが700以下
    if (transform.translate.y == 0.0f && boss_->GetHP() <= 700) {
        boss_->ChangeState(std::make_unique<AttackPhase3>(boss_));
    }

    //// フェーズ終了条件: HPが70%以下
    //if (boss_->GetHP() <= 700) {
    //    boss_->ChangeState(std::make_unique<AttackPhase3>(boss_));
    //}
}

void AttackPhase2::Draw() {
    if (shockWaveActive_) {
        for (const auto& object : shockWaveObjects_) {
            object->Draw();
        }
    }
}

void AttackPhase2::InitializeShockWaveObjects(size_t count) {
    shockWaveObjects_.clear();
    shockWaveObjects_.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        auto object = std::make_unique<Object3d>();
        object->Initialize();
        object->SetModel("ShockWave.obj"); // モデルを設定
        //object->SetModel("Player.obj"); // モデルを設定
        shockWaveObjects_.push_back(std::move(object));
    }
}

void AttackPhase2::ActivateShockWave() {
    shockWaveRadius_ = 10.0f; // 初期半径
    shockWaveActive_ = true;

    // ランダムスケール生成器の設定
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.0f);

    // 内側と外側の円の設定
    size_t halfSize = shockWaveObjects_.size() / 2; // オブジェクトを2つの円に分割
    float innerBaseRadius = shockWaveRadius_ * 1.5f; // 内側の半径
    float outerBaseRadius = shockWaveRadius_ * 2.5f; // 外側の半径

    // 内側の円の配置
    for (size_t i = 0; i < halfSize; ++i) {
        float angle = (kPI * 2.0f / halfSize) * i;
        float randomScale = scaleDist(gen);
        float x = boss_->GetTransform().translate.x + std::cos(angle) * innerBaseRadius;
        float z = boss_->GetTransform().translate.z + std::sin(angle) * innerBaseRadius;

        Object3d* object = shockWaveObjects_[i].get();
        object->SetScale({ randomScale, randomScale, randomScale });
        object->SetTranslate({ x, boss_->GetTransform().translate.y - 0.2f, z });
        object->SetRotate({ -45.0f, 45.0f, 0.0f }); // Y軸を45度回転
        object->Update();
    }

    // 外側の円の配置
    for (size_t i = halfSize; i < shockWaveObjects_.size(); ++i) {
        float angle = (kPI * 2.0f / (shockWaveObjects_.size() - halfSize)) * (i - halfSize);
        float randomScale = scaleDist(gen);
        float x = boss_->GetTransform().translate.x + std::cos(angle) * outerBaseRadius;
        float z = boss_->GetTransform().translate.z + std::sin(angle) * outerBaseRadius;

        Object3d* object = shockWaveObjects_[i].get();
        object->SetScale({ randomScale, randomScale, randomScale });
        object->SetTranslate({ x, boss_->GetTransform().translate.y - 0.2f, z });
        object->SetRotate({ 45.0f, -45.0f, 0.0f }); // Y軸を-45度回転
        object->Update();
    }
}


void AttackPhase2::UpdateShockWaveObjects() {
    // 内側と外側の広がる速度
    shockWaveRadius_ += 0.1f; // 広がる速度

    size_t halfSize = shockWaveObjects_.size() / 2;
    float innerBaseRadius = shockWaveRadius_ * 1.5f; // 内側の半径
    float outerBaseRadius = shockWaveRadius_ * 2.5f; // 外側の半径

    // 内側の円の更新（45度回転）
    for (size_t i = 0; i < halfSize; ++i) {
        float angle = (kPI * 2.0f / halfSize) * i;
        float x = boss_->GetTransform().translate.x + std::cos(angle) * innerBaseRadius;
        float z = boss_->GetTransform().translate.z + std::sin(angle) * innerBaseRadius;

        Object3d* object = shockWaveObjects_[i].get();
        object->SetTranslate({ x, boss_->GetTransform().translate.y - 0.2f, z });
        object->SetRotate({ -45.0f, 45.0f, 0.0f }); // Y軸を45度回転
        object->Update();
    }

    // 外側の円の更新（-45度回転）
    for (size_t i = halfSize; i < shockWaveObjects_.size(); ++i) {
        float angle = (kPI * 2.0f / (shockWaveObjects_.size() - halfSize)) * (i - halfSize);
        float x = boss_->GetTransform().translate.x + std::cos(angle) * outerBaseRadius;
        float z = boss_->GetTransform().translate.z + std::sin(angle) * outerBaseRadius;

        Object3d* object = shockWaveObjects_[i].get();
        object->SetTranslate({ x, boss_->GetTransform().translate.y - 0.2f, z });
        object->SetRotate({ 45.0f, -45.0f, 0.0f }); // Y軸を-45度回転
        object->Update();
    }

    // 最大半径に達したら終了
    if (shockWaveRadius_ >= maxRadius_) {
        shockWaveActive_ = false; // 衝撃波終了
    }
}





