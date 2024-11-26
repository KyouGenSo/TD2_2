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

	// ラインとの衝突判定を追加
	bool IntersectsLine(const Vector3& lineStart, const Vector3& lineDirection, float lineLength) const;

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
	 * \brief  GetColor
	 * \return
	 * \note
	 */
	Vector4 GetColor() { return color_; }

	/**----------------------------------------------------------------------------
	 * \brief  SetColor
	 * \param  color
	 * \note
	 */
	void SetColor(Vector4 color) { color_ = color; }

	/**----------------------------------------------------------------------------
	 * \brief  Intersects
	 * \param  other
	 * \return
	 * \note
	 */
	bool Intersects(const Collider& other) const;

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

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
};
