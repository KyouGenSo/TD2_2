#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include "Object3d.h"
#include <vector>
#include <memory>

enum class PhaseState {
    Ascending,    // 上昇中
    Descending,   // 下降中
    ShockWave,    // 衝撃波生成
    Reset         // 状態リセット
};

class AttackPhase2 : public BossAttackBaseState {
public:
    AttackPhase2(Boss* boss);

    void Update() override;
    void Draw() override;

private:
    void InitializeShockWaveObjects(size_t count);
    void ActivateShockWave();
    void UpdateShockWaveObjects();

    PhaseState state_ = PhaseState::Ascending; // 初期状態は上昇
    float shockWaveRadius_ = 0.0f;
    bool shockWaveActive_ = false;

    const float ascendingSpeed_ = 0.06f;  // 上昇速度
    const float descendingSpeed_ = 0.5f;  // 下降速度
    const float maxRadius_ = 100.0f;      // 衝撃波の最大半径
    const size_t shockWaveObjectCount_ = 30; // 衝撃波オブジェクト数
    std::vector<std::unique_ptr<Object3d>> shockWaveObjects_;
};

