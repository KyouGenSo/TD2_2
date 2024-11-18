/*********************************************************************
 * \file   Collider.cpp
 * \brief
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
/// \brief カプセルを描画する関数
/// \param color カプセルの色
/// \note この関数では、カプセルの線分部分と円形部分を描画します。
void Collider::DrawCapsule(const Vector4& color) {
    if(!drawEnabled_) return;

    const uint32_t kSubDivision = 16; // 円の分割数
    const float kLatEvery = float(M_PI) / float(kSubDivision);
    const float kLonEvery = 2.0f * float(M_PI) / float(kSubDivision);



    // DebugCameraのインスタンスを取得
    DebugCamera* camera = DebugCamera::GetInstance();
    Matrix4x4 viewProjectionMatrix = camera->GetViewProjectionMat();
    Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 1.0f);




    auto drawCircle = [&](const Vector3& center, float radius, bool isHorizontal) {
        for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
            float lon = lonIndex * kLonEvery;
            float nextLon = ( lonIndex + 1 ) * kLonEvery;

            Vector3 a, b;
            if(isHorizontal) {
                a = { cos(lon) * radius, 0.0f, sin(lon) * radius };
                b = { cos(nextLon) * radius, 0.0f, sin(nextLon) * radius };
            } else {
                a = { cos(lon) * radius, sin(lon) * radius, 0.0f };
                b = { cos(nextLon) * radius, sin(nextLon) * radius, 0.0f };
            }

            Vector3 screenA = Transform(AddVector3(a, center), viewProjectionMatrix);
            screenA = Transform(screenA, viewportMatrix);
            Vector3 screenB = Transform(AddVector3(b, center), viewProjectionMatrix);
            screenB = Transform(screenB, viewportMatrix);

            // ラインを描画
            Draw2D::GetInstance()->DrawLine(Vector2(screenA.x, screenA.y), Vector2(screenB.x, screenB.y), color);
        }
        };

    // 両端の円を描画
    drawCircle(Transform(start_, viewProjectionMatrix), radius_, true);
    drawCircle(Transform(end_, viewProjectionMatrix), radius_, true);

    // 中心線に沿った円柱部分を描画
    for(uint32_t latIndex = 0; latIndex <= kSubDivision / 2; ++latIndex) {
        float lat = float(M_PI) / 2.0f - latIndex * kLatEvery;
        float sinLat = sin(lat);
        float cosLat = cos(lat);

        for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
            float lon = lonIndex * kLonEvery;
            float nextLon = ( lonIndex + 1 ) * kLonEvery;

            Vector3 a = { cos(lon) * cosLat, sin(lon) * cosLat, sinLat };
            Vector3 b = { cos(nextLon) * cosLat, sin(nextLon) * cosLat, sinLat };

            a = AddVector3(MultiplyVector3(a, radius_), start_);
            b = AddVector3(MultiplyVector3(b, radius_), start_);
            Vector3 c = AddVector3(a, Vector3(end_.x - start_.x, end_.y - start_.y, end_.z - start_.z));
            Vector3 d = AddVector3(b, Vector3(end_.x - start_.x, end_.y - start_.y, end_.z - start_.z));

            Vector3 screenA = Transform(a, viewProjectionMatrix);
            screenA = Transform(screenA, viewportMatrix);
            Vector3 screenB = Transform(b, viewProjectionMatrix);
            screenB = Transform(screenB, viewportMatrix);
            Vector3 screenC = Transform(c, viewProjectionMatrix);
            screenC = Transform(screenC, viewportMatrix);
            Vector3 screenD = Transform(d, viewProjectionMatrix);
            screenD = Transform(screenD, viewportMatrix);

            // ラインを描画
            Draw2D::GetInstance()->DrawLine(Vector2(screenA.x, screenA.y), Vector2(screenB.x, screenB.y), color);
            Draw2D::GetInstance()->DrawLine(Vector2(screenA.x, screenA.y), Vector2(screenC.x, screenC.y), color);
            Draw2D::GetInstance()->DrawLine(Vector2(screenB.x, screenB.y), Vector2(screenD.x, screenD.y), color);
            Draw2D::GetInstance()->DrawLine(Vector2(screenC.x, screenC.y), Vector2(screenD.x, screenD.y), color);
        }
    }

    // カプセルの中心線（始点と終点を結ぶ線分）を描画
    Draw2D::GetInstance()->DrawLine(Vector2(Transform(start_, viewProjectionMatrix).x, Transform(start_, viewProjectionMatrix).y), Vector2(Transform(end_, viewProjectionMatrix).x, Transform(end_, viewProjectionMatrix).y), color);
}