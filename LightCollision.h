/*********************************************************************
 * \file   LightCollision.h
 * \brief ライトの当たり判定です
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Transform.h"

class LightCollision : public ObjectBase {

	///--------------------------------------------------------------
	///							メンバ関数
public:

	/// \brief 初期化
	void Initialize();

	/// \brief 更新
	void Update();

	/// \brief 描画 
	void Draw();

	/**----------------------------------------------------------------------------
	 * \brief  OnCollision
	 * \param  objectBase
	 * \note
	 */
	void OnCollision(ObjectBase* objectBase) override;

	///--------------------------------------------------------------
	///							静的メンバ関数
private:

	///--------------------------------------------------------------
	///							入出力関数
public:

	//開始位置のゲッター
	Vector3 GetStart() { return transformStart_; }
	//終了位置のゲッター
	Vector3 GetEnd() { return transformEnd_; }

	//開始位置のセッター
	void SetStart(Vector3 start) { transformStart_ = start; }
	//終了位置のセッター
	void SetEnd(Vector3 end) { transformEnd_ = end; }



	///--------------------------------------------------------------
	///							メンバ変数
private:
	//---------------------------------------
	// SRT
	//開始位置
	Vector3 transformStart_;
	//終了位置
	Vector3 transformEnd_;
	//オフセット
	Vector3 offset_;
};

