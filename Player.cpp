#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Camera.h"
#include <cmath>

void Player::Initialize(Boss* boss) {
    ModelManager::GetInstance()->LoadModel("Player.obj");
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    object3d_->SetModel("Player.obj");

    transform_.scale = { 5.0f, 5.0f, 5.0f };
    transform_.rotate = { 0.0f, 0.0f, 0.0f };
    transform_.translate = { 0.0f, 0.0f, -13.0f };

    boss_ = boss; // Boss のポインタを設定
}

void Player::Update() {
    if (boss_ == nullptr) return;

    // 左右移動の回転処理
    if (Input::GetInstance()->PushKey(DIK_A)) {
        angle_ -= rotationSpeed_; // 左回転
    }
    if (Input::GetInstance()->PushKey(DIK_D)) {
        angle_ += rotationSpeed_; // 右回転
    }

    // Boss の位置を中心に円状に移動
    const Transform& bossTransform = boss_->GetTransform();
    transform_.translate.x = bossTransform.translate.x + radius_ * cosf(angle_);
    transform_.translate.z = bossTransform.translate.z + radius_ * sinf(angle_);

    // Boss の方向を向くための角度計算
    Vector3 directionToBoss = bossTransform.translate - transform_.translate;
    transform_.rotate.y = atan2f(directionToBoss.x, directionToBoss.z);

    // ジャンプ処理
    if (Input::GetInstance()->PushKey(DIK_W) && !isJumping_) {
        isJumping_ = true;
        jumpVelocity_ = jumpPower_;
    }

    if (isJumping_) {
        transform_.translate.y += jumpVelocity_;
        jumpVelocity_ += gravity_;

        if (transform_.translate.y <= 0.0f) {
            transform_.translate.y = 0.0f;
            isJumping_ = false;
            jumpVelocity_ = 0.0f;
        }
    }

    // モデルの更新
    object3d_->SetScale(transform_.scale);
    object3d_->SetRotate(transform_.rotate);
    object3d_->SetTranslate(transform_.translate);
    object3d_->Update();

    // 追従カメラ
    Camera* camera = Object3dBasic::GetInstance()->GetCamera();
    if (camera) {
        Vector3 cameraOffset(0.0f, 5.0f, -15.0f); // プレイヤーの背後に配置するオフセット
        Vector3 cameraPos = transform_.translate + cameraOffset;
        camera->SetTranslate(cameraPos);

        // カメラがプレイヤーを向くための角度計算
        Vector3 directionToPlayer = transform_.translate - cameraPos;
        float cameraYAngle = atan2f(directionToPlayer.x, directionToPlayer.z);
        camera->SetRotate(Vector3(0.3f, cameraYAngle, 0.0f)); // Y 軸方向の角度のみ更新
    }
}

void Player::Draw() {
    object3d_->Draw();
}
