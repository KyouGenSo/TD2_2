/*********************************************************************
 * \file   Collider.cpp
 * \brief  カプセルの描画を行う関数
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#define NOMINMAX 
#include "Collider.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include "Draw2D.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "Object3dbasic.h"  
#include "ModelManager.h"
#include "Mat4x4Func.h" 
using namespace Mat4x4;
#include <algorithm>
#include <limits>

void Collider::Initialize() {
}

bool Collider::Intersects(const Collider& other) const {
    // 内部関数: 2つのベクトルのドット積を計算する
    auto dot = [](const Vector3& v1, const Vector3& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        };

    // 内部関数: 値を指定された範囲にクランプする
    auto clamp = [](float value, float min, float max) {
        return std::max(min, std::min(value, max));
        };

    // カプセル同士の衝突判定
    Vector3 d1 = end_ - start_; // thisカプセルの線分ベクトル
    Vector3 d2 = other.end_ - other.start_; // otherカプセルの線分ベクトル
    Vector3 r = start_ - other.start_; // thisカプセルの開始点とotherカプセルの開始点の差

    float a = dot(d1, d1); // thisカプセルの線分ベクトルの長さの二乗
    float e = dot(d2, d2); // otherカプセルの線分ベクトルの長さの二乗
    float f = dot(d2, r); // otherカプセルの線分ベクトルとrのドット積

    float s, t; // パラメータsとtの初期化
    if(a <= std::numeric_limits<float>::epsilon() && e <= std::numeric_limits<float>::epsilon()) {
        // 両方のカプセルが線分ではなく点の場合
        s = t = 0.0f;
    } else if(a <= std::numeric_limits<float>::epsilon()) {
        // thisカプセルが線分ではなく点の場合
        s = 0.0f;
        t = f / e;
        t = clamp(t, 0.0f, 1.0f);
    } else {
        float c = dot(d1, r); // thisカプセルの線分ベクトルとrのドット積
        if(e <= std::numeric_limits<float>::epsilon()) {
            // otherカプセルが線分ではなく点の場合
            t = 0.0f;
            s = clamp(-c / a, 0.0f, 1.0f);
        } else {
            float b = dot(d1, d2); // thisカプセルの線分ベクトルとotherカプセルの線分ベクトルのドット積
            float denom = a * e - b * b; // 分母の計算
            if(denom != 0.0f) {
                s = clamp(( b * f - c * e ) / denom, 0.0f, 1.0f);
            } else {
                s = 0.0f;
            }
            t = ( b * s + f ) / e;
            if(t < 0.0f) {
                t = 0.0f;
                s = clamp(-c / a, 0.0f, 1.0f);
            } else if(t > 1.0f) {
                t = 1.0f;
                s = clamp(( b - c ) / a, 0.0f, 1.0f);
            }
        }
    }

    // 最も近い点を計算
    Vector3 closestPoint1 = start_ + d1 * s;
    Vector3 closestPoint2 = other.start_ + d2 * t;

    // closestPoint1とclosestPoint2の距離を計算する
    float distance = ( closestPoint1 - closestPoint2 ).length();

    // 距離が半径の和以下であれば衝突していると判定する
    return distance <= ( radius_ + other.radius_ );
}