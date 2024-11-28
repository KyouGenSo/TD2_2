/*********************************************************************
 * \file   Player.h
 * \brief  プレイヤークラス
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include "Boss.h"
#include "Object3d.h"
#include "FollowCamera.h"
#include "Vector3.h"
#include "Light.h"
#include "ObjectBase.h"
#include <memory>
#include "LightCollision.h"
#include "Sprite.h"
#include <vector>
//コントローラー操作
#include <XInput.h>
#pragma comment(lib, "Xinput.lib")


class Player :public ObjectBase {
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

	void Light();

	/// \brief ImGuiの描画
	void DrawImGui();

	/// @brief 衝突処理イベント
	/// @param objectBase 衝突したオブジェクト
	void OnCollision(ObjectBase* objectBase) override;

	/// スプライト描画関数
	void DrawSprite();


	///--------------------------------------------------------------
	///							入出力関数
public:

	Transform& GetTransform() { return transform_; }

	// プレイヤーの現在位置を取得
	Vector3 GetPosition() const { return transform_.translate; }

	//ライトの当たり判定のゲッター
	LightCollision* GetLightCollision() { return lightCollision_.get(); }

	bool GetisTutorial() { return isTutorial_; }

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
	const float radius_ = 22.5f; // Boss を中心とする円の半径
	const float rotationSpeed_ = 0.04f; // 回転速度


	std::unique_ptr<FollowCamera> followCamera_;

	bool isTutorial_ = false;        // チュートリアル中かどうか
	float tutorialElapsedTime_ = 0; // チュートリアル経過時間

	std::unique_ptr<Sprite> tutorialSprite_; // チュートリアル用スプライト


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


	int actionDelay_ = 180; // 行動制限時間（フレーム数）
	bool canAct_ = false;  // プレイヤーが行動可能かどうか

	//ライトの当たり判定(ユニークポインタ)
	std::unique_ptr<LightCollision> lightCollision_ = nullptr;

private:
	// パーティクル管理用の構造体
	struct DustParticle {
		std::unique_ptr<Object3d> object; // パーティクルオブジェクト
		Vector3 velocity;                // 速度
		int lifetime;                    // 寿命（フレーム数）
	};

	std::vector<DustParticle> dustParticles_; // 砂埃パーティクルのリスト
	void GenerateDust();                     // 砂埃を生成する関数


	XINPUT_STATE controllerState_{}; 

	// HP 関連のメンバ変数
	int hp_ = 200;                    // 現在の HP
	const int maxHp_ = 200;           // 最大 HP
	Vector2 hpBarPosition_ = { 50.0f, 300.0f }; // HP バーの描画位置 (画面の上に移動)
	Vector2 hpBarSize_ = { 20.0f, 300.0f };     // HP バーのサイズ (縦長)
	Vector4 hpBarColor_ = { 0.0f, 1.0f, 0.0f, 1.0f }; // HP バーの色


public:

	// HP の描画関数
	void DrawHPBar();
	void UpdateHP();

};

