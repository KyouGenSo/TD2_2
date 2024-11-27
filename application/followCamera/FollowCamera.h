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
    float followSpeed_ = 0.02f;  // 補間速度（初期値は 0.02f）
    float cameraDistance_ = 25.0f; // カメラ距離
    float cameraHeight_ = 1.85f;    // カメラ高さ
    float lookUpAngle_ = 0.1f;     // 見上げる角度（ラジアン）
    int frameCounter_ = 0;         // フレームカウンター
    int transitionTime_ = 180;      // 補間速度切り替えまでの時間（フレーム数）

    //Vector3 cameraPos;
    Vector3 currentShakeOffset_ = { 0.0f, 0.0f, 0.0f }; // シェイクのオフセット

private:
    bool isShaking_ = false;
    int shakeDuration_ = 30; // シェイクのフレーム数
    int shakeFrameCount_ = 0;
    float shakeIntensity_ = 0.5f; // シェイクの強さ

public:
    void StartShake(int duration, float intensity);
    void UpdateShake();
};
