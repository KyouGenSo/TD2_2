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

void Collider::Initialize() {
}

bool Collider::IntersectsLine(const Vector3& lineStart, const Vector3& lineDirection, float lineLength) const
{
	// ラインの終了点を計算
	Vector3 lineEnd = lineStart + lineDirection.normalize() * lineLength;

	// ラインの方向ベクトルと長さ
	Vector3 lineDir = (lineEnd - lineStart).normalize();

	// カプセルの中心線方向ベクトルと長さ
	Vector3 capsuleDir = (end_ - start_).normalize();
	float capsuleLength = (end_ - start_).Length();

	// ラインの最近接点を計算
	float t = std::clamp((start_ - lineStart).dot(lineDir), 0.0f, lineLength);
	Vector3 closestPointOnLine = lineStart + lineDir * t;

	// カプセルの最近接点を計算
	float u = std::clamp((closestPointOnLine - start_).dot(capsuleDir), 0.0f, capsuleLength);
	Vector3 closestPointOnCapsule = start_ + capsuleDir * u;

	// 最近接点間の距離を計算
	float distanceSquared = (closestPointOnLine - closestPointOnCapsule).LengthSquared();

	// 距離がカプセルの半径以下なら衝突している
	return distanceSquared <= (radius_ * radius_);
}

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

	// closestPoint2を更新する
	if(other.start_.x < start_.x) closestPoint2.x = start_.x;
	else if(other.start_.x > end_.x) closestPoint2.x = end_.x;
	if(other.start_.y < start_.y) closestPoint2.y = start_.y;
	else if(other.start_.y > end_.y) closestPoint2.y = end_.y;
	if(other.start_.z < start_.z) closestPoint2.z = start_.z;
	else if(other.start_.z > end_.z) closestPoint2.z = end_.z;

	// closestPoint1とclosestPoint2の距離を計算する
	float distance = std::sqrt(
		( closestPoint1.x - closestPoint2.x ) * ( closestPoint1.x - closestPoint2.x ) +
		( closestPoint1.y - closestPoint2.y ) * ( closestPoint1.y - closestPoint2.y ) +
		( closestPoint1.z - closestPoint2.z ) * ( closestPoint1.z - closestPoint2.z )
	);

	// 距離が半径の和以下であれば衝突していると判定する
	return distance <= ( radius_ + other.radius_ );
}