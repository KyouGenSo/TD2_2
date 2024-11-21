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
	if(start_.x < other.start_.x) closestPoint1.x = other.start_.x;
	else if(start_.x > other.end_.x) closestPoint1.x = other.end_.x;
	if(start_.y < other.start_.y) closestPoint1.y = other.start_.y;
	else if(start_.y > other.end_.y) closestPoint1.y = other.end_.y;
	if(start_.z < other.start_.z) closestPoint1.z = other.start_.z;
	else if(start_.z > other.end_.z) closestPoint1.z = other.end_.z;

	// closestPoint1とclosestPoint2の距離を計算する
	float distance = std::sqrt(
		( closestPoint1.x - closestPoint2.x ) * ( closestPoint1.x - closestPoint2.x ) +
		( closestPoint1.y - closestPoint2.y ) * ( closestPoint1.y - closestPoint2.y ) +
		( closestPoint1.z - closestPoint2.z ) * ( closestPoint1.z - closestPoint2.z )
	);

	// 距離が半径の和以下であれば衝突していると判定する
	return distance <= ( radius_ + other.radius_ );
}