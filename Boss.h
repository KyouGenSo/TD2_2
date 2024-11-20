#pragma once
#include <memory>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Mat4x4Func.h"
#include "Draw2D.h"
#include "BossAttackBaseState.h"

class Object3d;

class Boss {
public: // メンバ関数

    // 初期化
    void Initialize();

    // 更新
    void Update();

    // 描画
    void Draw();

    // ImGui描画
    void DrawImGui();

    // HP描画
    void HPUpdate();

    // HP更新
    void HPDraw();

    // ステート変更
    void ChangeState(std::unique_ptr<BossAttackBaseState> state);

    // Player位置をセット
    void SetPlayerPosition(const Vector3& playerPosition) {
        playerPosition_ = playerPosition;
    }

    Transform& GetTransform() { return transform_; }
    uint32_t GetHP() const { return hp_; }

private:// メンバ変数

    std::unique_ptr<Object3d> object3d_ = nullptr;
    Transform transform_;
    Vector3 playerPosition_;
    uint32_t hp_ = 1000;

    Vector2 boxPosition = { 140.0f, 10.0f };
    Vector2 boxSize = { 1000.0f, 25.0f };
    Vector4 boxColor = { 0.0f, 1.0f, 0.0f, 1.0f };


    // 現在の状態
    std::unique_ptr<BossAttackBaseState> state_;

};
