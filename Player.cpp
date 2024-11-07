#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Camera.h"
#include <cmath>

void Player::Initialize(Boss* boss) {
    // プレイヤーモデルの読み込みと設定
    ModelManager::GetInstance()->LoadModel("Player.obj");
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    object3d_->SetModel("Player.obj");

    // スケール、回転、位置の初期設定
    transform_.scale = { 5.0f, 5.0f, 5.0f };
    transform_.rotate = { 0.0f, 0.0f, 0.0f };
    transform_.translate = { 0.0f, 0.0f, -13.0f };

    boss_ = boss; // Boss のポインタを設定
}

void Player::Update() {
    if (boss_ == nullptr) return;

    // プレイヤーの左右移動 (Boss の周りを回転)
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

    // プレイヤーの背後に追従するカメラ
    Camera* camera = Object3dBasic::GetInstance()->GetCamera();
    if (camera) {
        // プレイヤーの回転角度に合わせたカメラのオフセットを計算
        float cameraDistance = 25.0f; // プレイヤーからカメラまでの距離を少し遠めに
        float cameraHeight = 2.0f;    // カメラの高さを低めに設定

        // プレイヤーの背後の位置を計算
        float offsetX = cameraDistance * sinf(transform_.rotate.y);
        float offsetZ = cameraDistance * cosf(transform_.rotate.y);

        Vector3 cameraPos;
        cameraPos.x = transform_.translate.x - offsetX;
        cameraPos.y = transform_.translate.y + cameraHeight;
        cameraPos.z = transform_.translate.z - offsetZ;

        camera->SetTranslate(cameraPos);

        // カメラが見上げるように角度を調整
        Vector3 lookAt = transform_.translate;
        Vector3 directionToPlayer = lookAt - cameraPos;
        float cameraYAngle = atan2f(directionToPlayer.x, directionToPlayer.z);

        // X軸の回転角を負の値にしてカメラを見上げるように設定
        camera->SetRotate(Vector3(-0.08f, cameraYAngle, 0.0f));
    }
}

void Player::Draw() {
    // モデルの描画
    object3d_->Draw();
}
