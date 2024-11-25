#include "AttackPhase2.h"
#include <AttackPhase3.h>
#include <cmath>

constexpr float kPI = 3.14159265358979323846f;

AttackPhase2::AttackPhase2(Boss* boss) : BossAttackBaseState("Phase2", boss)
{
    InitializeShockWaveSprites(12); // 必要なスプライト数を事前生成
}

void AttackPhase2::Update()
{
    Transform& transform = boss_->GetTransform();
    transform.scale = { 1.0f, 1.0f, 1.0f };

    // クールダウン中の処理
    if (isCooldown_)
    {
        ++cooldownTimer_;
        if (cooldownTimer_ >= cooldownDuration_)
        {
            isCooldown_ = false; // クールダウン終了
            cooldownTimer_ = 0; // タイマーリセット
        }
        return; // クールダウン中は何もしない
    }

    ++JumpCounter_;

    if (JumpCounter_ >= 180)
    {
        Attack();
    }

    // フェーズ終了条件: HPが50%以下
    if (boss_->GetHP() <= 750)
    {
        boss_->ChangeState(std::make_unique<AttackPhase3>(boss_));
    }

    // スプライトの広がりアニメーション更新
    UpdateShockWaveSprites();
}

void AttackPhase2::Draw()
{
    for (const auto& sprite : shockWaveSprites_)
    {
        sprite->Draw();
    }
}

void AttackPhase2::Attack()
{
    Transform& transform = boss_->GetTransform();

    if (isAscending_) // 上昇中
    {
        transform.translate.y += 0.06f;
        if (transform.translate.y >= 7.0f)
        {
            isAscending_ = false; // 上昇完了後に下降に切り替える
        }
    }
    else // 下降中
    {
        transform.translate.y -= 0.5f;
        if (transform.translate.y <= 0.0f)
        {
            transform.translate.y = 0.0f; // 地面に到達したら停止
            isAscending_ = true; // 次の動きで再び上昇できるようにする
            isCooldown_ = true; // クールダウン開始

            // 着地時にスプライトをリセットして円状に広げる
            ResetShockWaveSprites(transform.translate.x, transform.translate.y);
        }
    }
}

void AttackPhase2::InitializeShockWaveSprites(size_t count)
{
    shockWaveSprites_.clear();
    shockWaveSprites_.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
        sprite->Initialize("/resources/Texture/ShockWave.png"); // 初期化はここだけ
        shockWaveSprites_.push_back(std::move(sprite));
    }
}

void AttackPhase2::ResetShockWaveSprites(float centerX, float centerY)
{
    constexpr int kNumSprites = 12; // スプライトの数
    constexpr float kRadius = 50.0f; // 初期の円の半径

    for (int i = 0; i < kNumSprites && i < shockWaveSprites_.size(); ++i)
    {
        float angle = (kPI * 2.0f / kNumSprites) * i;
        float x = centerX + std::cos(angle) * kRadius;
        float y = centerY + std::sin(angle) * kRadius;

        Sprite* sprite = shockWaveSprites_[i].get();
        sprite->SetPos({ x, y });
        sprite->SetSize({ 32.0f, 32.0f }); // スプライトのサイズをリセット
        sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 完全に不透明にリセット
    }

    shockWaveRadius_ = kRadius;
    shockWaveExpansionSpeed_ = 2.0f; // スプライトの広がる速度
}

void AttackPhase2::UpdateShockWaveSprites()
{
    shockWaveRadius_ += shockWaveExpansionSpeed_;

    for (auto& sprite : shockWaveSprites_)
    {
        float angle = (kPI * 2.0f / shockWaveSprites_.size()) * (&sprite - &shockWaveSprites_[0]);
        float x = boss_->GetTransform().translate.x + std::cos(angle) * shockWaveRadius_;
        float y = boss_->GetTransform().translate.y + std::sin(angle) * shockWaveRadius_;

        sprite->SetPos({ x, y });
        sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f - (shockWaveRadius_ / 200.0f) }); // 徐々に透明に
    }

    // 一定距離を超えたら透明にする
    if (shockWaveRadius_ > 200.0f)
    {
        for (auto& sprite : shockWaveSprites_)
        {
            sprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 完全に透明化
        }
    }
}
