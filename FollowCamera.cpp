#include "FollowCamera.h"
#include <cmath>

FollowCamera::FollowCamera() : previousPosition_(Vector3(0.0f, 0.0f, 0.0f)), frameCounter_(0) {}

void FollowCamera::Update(const Vector3& playerPosition, const Vector3& playerRotation) {
    Camera* camera = Object3dBasic::GetInstance()->GetCamera();
    if (!camera) return;

    // フレームカウントを進める
    frameCounter_++;

    // フレーム数に応じて補間速度を切り替える
    if (frameCounter_ <= transitionTime_) {
        followSpeed_ = 0.02f; // 最初の transitionTime_ フレーム
    }
    else {
        followSpeed_ = 0.2f; // transitionTime_ を超えたら速度を上げる
    }

    // プレイヤーの過去の位置を更新 (Lerp を使用)
    previousPosition_ = Lerp(previousPosition_, playerPosition, followSpeed_);

    // カメラの追従位置を計算
    float offsetX = cameraDistance_ * sinf(playerRotation.y);
    float offsetZ = cameraDistance_ * cosf(playerRotation.y);

    Vector3 cameraPos;
    cameraPos.x = previousPosition_.x - offsetX;
    cameraPos.y = previousPosition_.y + cameraHeight_;
    cameraPos.z = previousPosition_.z - offsetZ;

    camera->SetTranslate(cameraPos);

    // カメラの回転を設定
    Vector3 lookAt = previousPosition_;
    Vector3 directionToPlayer = lookAt - cameraPos;
    float cameraYAngle = atan2f(directionToPlayer.x, directionToPlayer.z);

    camera->SetRotate(Vector3(-0.15f, cameraYAngle, 0.0f));
}
