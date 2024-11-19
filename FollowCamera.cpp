#include "FollowCamera.h"
#include <cmath>

FollowCamera::FollowCamera() : previousPosition_(Vector3(0.0f, 0.0f, 0.0f)) {}

void FollowCamera::Update(const Vector3& playerPosition, const Vector3& playerRotation) {
    Camera* camera = Object3dBasic::GetInstance()->GetCamera();
    if (!camera) return;

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

    camera->SetRotate(Vector3(-0.08f, cameraYAngle, 0.0f));
}
