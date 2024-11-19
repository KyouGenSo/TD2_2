/*********************************************************************
 * \file   ObjectBase.h
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include <memory>
#include "Model.h"
#include "Vector3.h"
#include "Mat4x4Func.h"
#include "Collider.h"

class ObjectBase {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Init( Vector3& startPoint,  Vector3& endPoint, float radius);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3& startPoint, Vector3& endPoint);

	/// @brief
	void Draw();

	/// <summary>
	/// 衝突処理イベント
	/// </summary>
	virtual void OnCollision(ObjectBase* objectBase) = 0;

	/// <summary>
	/// コライダーのゲッター
	/// </summary>
	/// <returns>コライダー</returns>
	std::unique_ptr<Collider>& GetCollider() { return collider_; }


protected:
	// コライダー
	std::unique_ptr<Collider> collider_;

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;
};