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
	void Initialize(Boss* boss);

	/// \brief 更新
	void Update();

	/// \brief 描画 
	void Draw();

	/// \brief 移動
	void Move();

	/// \brief カメラ
	void FollowCamera();

	/// \brief ImGuiの描画
	void DrawImGui();

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

	Boss* boss_ = nullptr; // Boss クラスへのポインタ
	float angle_ = 0.0f;   // 現在の角度
	const float radius_ = 12.5f; // Boss を中心とする円の半径
	const float rotationSpeed_ = 0.04f; // 回転速度

	// light関連変数
	Vector3 lightPos_;
	Vector3 lightDir_;
	Vector4 lightColor_;
	float lightIntensity_;
	float lightRange_;
	float lightDecay_;
	float lightSpotAngle_;
	bool isSpotLight_;
};

