/*********************************************************************
 * \file   Player.h
 * \brief  プレイヤークラス
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include"Sprite.h"
#include"Object3d.h"
#include"Vector3.h"
#include <memory>
#include "Input.h"
#include "Boss.h"

class Player {
	///--------------------------------------------------------------
	///							メンバ関数
public:

	/// \brief 初期化
	void Initialize();

	/// \brief 更新
	void Update();

	/// \brief 描画 
	void Draw();

	///--------------------------------------------------------------
	///							入出力関数
public:


	///--------------------------------------------------------------
	///							メンバ変数
private:
	//---------------------------------------
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;

	//---------------------------------------
	// SRT
	Transform transform_;

	// ジャンプ関連の変数
	bool isJumping_ = false;
	float jumpVelocity_ = 0.0f;
	const float gravity_ = -0.02f;
	const float jumpPower_ = 0.4f;

};

