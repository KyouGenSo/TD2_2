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

	// スケール、回転、位置の初期設定
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	// モデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("capsuleHead.obj");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("capsuleHead.obj");
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

///--------------------------------------------------------------
/// \brief カプセルを描画する/関数
/// \param color カプセルの色
void Collider::DrawCapsule() {
	///カプセルの描画
	object3d_->Draw();

	//// テスト用の直線を描画する
	//Draw2D::GetInstance()->DrawLine(Vector2(0, 0), Vector2(100, 100), Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	//// 早期リターン
	//if(!drawEnabled_) return;

	//// 半径が0以下の場合エラー
	//assert(radius_ > 0.0f);

	//// 無効な座標のチェック
	//assert(!std::isnan(start_.x) && !std::isnan(start_.y) && !std::isnan(start_.z));
	//assert(!std::isnan(end_.x) && !std::isnan(end_.y) && !std::isnan(end_.z));

	//const uint32_t kSubDivision = 16; // 円の分割数
	//const float kLatEvery = static_cast<float>( M_PI ) / kSubDivision;
	//const float kLonEvery = 2.0f * static_cast<float>( M_PI ) / kSubDivision;

	//// ビュー行列、プロジェクション行列、ワールド行列、ビュープロジェクション行列を取得
	//Matrix4x4 viewMatrix = Draw2D::GetInstance()->GetViewMatrix();
	//Matrix4x4 projectionMatrix = Draw2D::GetInstance()->GetProjectionMatrix();
	//Matrix4x4 worldMatrix = Draw2D::GetInstance()->GetWorldMatrix();

	//Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	//Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

	//Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 1.0f);


	//// ローカル関数の定義
	//auto drawCircle = [&](const Vector3& center, float radius, bool isHorizontal) {
	//	for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
	//		float lon = lonIndex * kLonEvery;
	//		float nextLon = ( lonIndex + 1 ) * kLonEvery;

	//		Vector3 a, b;
	//		if(isHorizontal) {
	//			a = { cosf(lon) * radius, 0.0f, sinf(lon) * radius };
	//			b = { cosf(nextLon) * radius, 0.0f, sinf(nextLon) * radius };
	//		} else {
	//			a = { cosf(lon) * radius, sinf(lon) * radius, 0.0f };
	//			b = { cosf(nextLon) * radius, sinf(nextLon) * radius, 0.0f };
	//		}

	//		Vector3 screenA = Transform(Transform(AddVector3(a, center), worldViewProjectionMatrix), viewportMatrix);
	//		Vector3 screenB = Transform(Transform(AddVector3(b, center), worldViewProjectionMatrix), viewportMatrix);

	//		Draw2D::GetInstance()->DrawLine(Vector2(screenA.x, screenA.y), Vector2(screenB.x, screenB.y), color);
	//	}
	//	};


	//// カプセルの上下の円を描画する
	//drawCircle(Transform(start_, worldViewProjectionMatrix), radius_, true);
	//drawCircle(Transform(end_, worldViewProjectionMatrix), radius_, true);

	//// カプセルの側面を描画する
	//for(uint32_t latIndex = 0; latIndex <= kSubDivision / 2; ++latIndex) {
	//	float lat = static_cast<float>( M_PI ) / 2.0f - latIndex * kLatEvery;
	//	float sinLat = sinf(lat);
	//	float cosLat = cosf(lat);

	//	for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
	//		float lon = lonIndex * kLonEvery;
	//		float nextLon = ( lonIndex + 1 ) * kLonEvery;

	//		Vector3 a = { cosf(lon) * cosLat, sinf(lon) * cosLat, sinLat };
	//		Vector3 b = { cosf(nextLon) * cosLat, sinf(nextLon) * cosLat, sinLat };

	//		a = AddVector3(MultiplyVector3(a, radius_), start_);
	//		b = AddVector3(MultiplyVector3(b, radius_), start_);
	//		Vector3 c = AddVector3(a, Vector3(end_.x - start_.x, end_.y - start_.y, end_.z - start_.z));
	//		Vector3 d = AddVector3(b, Vector3(end_.x - start_.x, end_.y - start_.y, end_.z - start_.z));

	//		Vector3 screenA = Transform(Transform(a, worldViewProjectionMatrix), viewportMatrix);
	//		Vector3 screenB = Transform(Transform(b, worldViewProjectionMatrix), viewportMatrix);
	//		Vector3 screenC = Transform(Transform(c, worldViewProjectionMatrix), viewportMatrix);
	//		Vector3 screenD = Transform(Transform(d, worldViewProjectionMatrix), viewportMatrix);

	//		// カプセルの側面を描画する
	//		Draw2D::GetInstance()->DrawLine(Vector2(screenA.x, screenA.y), Vector2(screenB.x, screenB.y), color);
	//		Draw2D::GetInstance()->DrawLine(Vector2(screenC.x, screenC.y), Vector2(screenD.x, screenD.y), color);
	//	}
	//}
}