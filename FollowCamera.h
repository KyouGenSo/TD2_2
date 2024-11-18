#pragma once
#include "Camera.h"
#include "Vector3.h"
#include "Object3dBasic.h"

class FollowCamera {
public:
    FollowCamera();
    ~FollowCamera() = default;

    void Update(const Vector3& playerPosition, const Vector3& playerRotation);

    Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
    }


private:
    Vector3 previousPosition_; // プレイヤーの過去の位置
    float followSpeed_ = 0.15f; // 補間速度
    float cameraDistance_ = 25.0f; // カメラ距離
    float cameraHeight_ = 2.0f;    // カメラ高さ
};
