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
#include "FollowCamera.h"

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


	std::unique_ptr<FollowCamera> followCamera_;

	////ライト関連のコード-------------------------

	struct LightProfile {
		Vector3 lightPos;
		Vector3 lightDir;
		Vector4 lightColor;
		float lightIntensity;
		float lightRange;
		float lightDecay;
		float lightSpotAngle;
		bool isSpotLight;
	};

	// ライトプロファイル
	LightProfile narrowStrongLight_;  // 範囲が狭く光が強い
	LightProfile wideWeakLight_;      // 範囲が広く光が弱い
	LightProfile* currentLight_;      // 現在のライト設定を指すポインタ

	// キー切り替えフラグ
	bool isLightProfileToggled_ = false;
	// ライトの方向を自動更新するかどうかのフラグ
	bool autoUpdateLightDir_ = true;

	////----------------------------------------

};

