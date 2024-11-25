#pragma once
#include "Boss.h"
#include "BossAttackBaseState.h"
#include <Sprite.h>

class AttackPhase2 : public BossAttackBaseState
{
public:
    AttackPhase2(Boss* boss);

    void Update() override;

    void Draw() override;

    void Attack();

private:
    // スプライト生成と更新
    void PrepareShockWaveSprites(size_t count);
    void CreateShockWaveSprites(float centerX, float centerY);
    void UpdateShockWaveSprites();


private:

    uint32_t JumpCounter_ = 0;
    bool isAscending_ = true;        // 上昇中か下降中かを管理するフラグ
    bool isCooldown_ = false;       // クールダウン中かを管理するフラグ
    uint32_t cooldownTimer_ = 0;    // クールダウンのカウント
    const uint32_t cooldownDuration_ = 120; // クールダウンの時間（フレーム数）

    // ShockWave関連
    std::vector<std::unique_ptr<Sprite>> reusableShockWaveSprites_; // 再利用可能なスプライトリスト
    float shockWaveRadius_ = 0.0f;      // 円の半径
    float shockWaveExpansionSpeed_ = 2.0f; // スプライトが広がる速度
    
};

