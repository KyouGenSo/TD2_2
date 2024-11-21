/*********************************************************************
 * \file   Collider.cpp
 * \brief  カプセルの描画を行う関数
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#include "Collider.h"
#define _USE_MATH_DEFINES
#include <algorithm>
#include <math.h>
#include <cmath>
#include "Draw2D.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "Object3dbasic.h"
#include "ModelManager.h"
#include "Mat4x4Func.h" 
using namespace Mat4x4;

void Collider::Initialize() {
}

///--------------------------------------------------------------
/// \brief カプセル同士の衝突判定を行う関数
/// \param other 衝突判定を行う対象のColliderオブジェクト
/// \return 衝突している場合はtrue、そうでない場合はfalse
bool Collider::Intersects(const Collider& other) const {
    // カプセル同士の衝突判定
    Vector3 closestPoint1 = start_;
    Vector3 closestPoint2 = other.start_;

    // closestPoint1を更新する
	// NOTE:std::clampとは、第一引数が第二引数と第三引数の範囲内に収まるように調整する関数である！
    closestPoint1.x = std::clamp(start_.x, other.start_.x, other.end_.x);
    closestPoint1.y = std::clamp(start_.y, other.start_.y, other.end_.y);
    closestPoint1.z = std::clamp(start_.z, other.start_.z, other.end_.z);

    // closestPoint1とclosestPoint2の距離の二乗を計算する
    float distanceSquared = 
        (closestPoint1.x - closestPoint2.x) * (closestPoint1.x - closestPoint2.x) +
        (closestPoint1.y - closestPoint2.y) * (closestPoint1.y - closestPoint2.y) +
        (closestPoint1.z - closestPoint2.z) * (closestPoint1.z - closestPoint2.z);

    // 距離の二乗が半径の和の二乗以下であれば衝突していると判定する
    float radiusSum = radius_ + other.radius_;
    return distanceSquared <= (radiusSum * radiusSum);
}