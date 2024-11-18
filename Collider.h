/*********************************************************************
 * \file   Collider.h
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include "Model.h"
#include "Vector3.h"
#include "Mat4x4Func.h"
#include <memory>
#include "Object3d.h"

 ///=============================================================================
 ///						クラス定義
class Collider {
	///--------------------------------------------------------------
	///						 メンバ関数
public:
	/**
	 * \brief 初期化
	 */
	void Initialize();

	///--------------------------------------------------------------
	///						 入出力関数　
public:
	/**----------------------------------------------------------------------------
	 * \brief  GetStart カプセルの開始座標を取得
	 * \return
	 * \note
	 */
	Vector3 GetStart() { return start_; }
	/**----------------------------------------------------------------------------
	 * \brief  GetEnd カプセルの終了座標を取得
	 * \return
	 * \note
	 */
	Vector3 GetEnd() { return end_; }

	/**----------------------------------------------------------------------------
	 * \brief  SetStart カプセルの開始座標を設定
	 * \param  start
	 * \note
	 */
	void SetStart(Vector3 start) { start_ = start; }
	/**----------------------------------------------------------------------------
	 * \brief  SetEnd
	 * \param  end
	 * \note
	 */
	void SetEnd(Vector3 end) { end_ = end; }

	/**----------------------------------------------------------------------------
	 * \brief  GetRadius 半径を取得
	 * \return
	 * \note
	 */
	float GetRadius() { return radius_; }

	/**----------------------------------------------------------------------------
	 * \brief  SetRadius 半径を設定
	 * \param  radius
	 * \note
	 */
	void SetRadius(float radius) { radius_ = radius; }

	/**----------------------------------------------------------------------------
	 * \brief  Intersects
	 * \param  other
	 * \return
	 * \note
	 */
	bool Intersects(const Collider& other) const;


	/**----------------------------------------------------------------------------
	 * \brief  DrawCapsule カプセルの描画
	 * \param  color
	 * \note
	 */
	void DrawCapsule();

	/**----------------------------------------------------------------------------
	 * \brief  SetDrawEnabled 描画のオン/オフ
	 * \param  enabled
	 * \note
	 */
	void SetDrawEnabled(bool enabled) { drawEnabled_ = enabled; }
	/**----------------------------------------------------------------------------
	 * \brief  IsDrawEnabled
	 * \return
	 * \note
	 */
	bool IsDrawEnabled() const { return drawEnabled_; }

	///--------------------------------------------------------------
	///						 
	// ローカル関数の定義
	Vector3 AddVector3(const Vector3& a, const Vector3& b) {
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}

	Vector3 MultiplyVector3(const Vector3& v, float scalar) {
		return { v.x * scalar, v.y * scalar, v.z * scalar };
	}

	//Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
	//	return {
	//		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
	//		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
	//		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]
	//	};
	//}

	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
		// 単位行列で初期化
		Matrix4x4 result = IdentityMatrix();

		// ビューポート変換行列の要素を計算する
		result.m[0][0] = width / 2.0f;
		result.m[1][1] = -height / 2.0f;
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + width / 2.0f;
		result.m[3][1] = top + height / 2.0f;
		result.m[3][2] = minDepth;

		return result;
	}

	Matrix4x4 IdentityMatrix() {
		Matrix4x4 result;
		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				if(i == j) {
					result.m[i][j] = 1.0f;
				} else {
					result.m[i][j] = 0.0f;
				}
			}
		}
		return result;
	}


	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:
	/// ===カプセルの開始座標=== ///
	Vector3 start_ = { 0.0f, 0.0f, 0.0f };

	/// ===カプセルの終了座標=== ///
	Vector3 end_ = { 0.0f, 0.0f, 0.0f };

	/// ===衝突範囲=== ///
	float radius_ = 1.0f;

	bool drawEnabled_ = true; // 描画のオン/

	//---------------------------------------
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;

	//---------------------------------------
	// SRT
	Transform transform_;
};
