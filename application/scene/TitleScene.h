#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include <vector>
#include"Vector2.h"
#include <memory>
#include "imgui.h"
#include "Object3d.h"
#include "Transform.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Skydome.h"

class TitleScene : public BaseScene
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui() override;

	// ライトの更新
	void Light();
private:

	// 描画ループ内で使用
	void ShowFPS() {
		ImGui::Begin("FPS Display"); // ウィンドウを開始
		ImGuiIO& io = ImGui::GetIO(); // ImGuiのIOオブジェクトを取得
		ImGui::Text("FPS: %.1f", io.Framerate); // FPSを表示
		ImGui::End(); // ウィンドウを終了
	}

private: // メンバ変数

	bool isDebug_ = false;

	std::unique_ptr<Sprite> DebugSprite_;  // 背景スプライト

	//========================================
	// タイトルオブジェクト
	std::unique_ptr<Object3d> titleObject_ = nullptr;
	// タイトルのTransform
	Transform titleTransform_;

	//========================================
	// 通常カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//========================================
	// 地面
	std::unique_ptr<Ground> ground_;

	//========================================
	// スカイドーム
	std::unique_ptr<Skydome> skydome_;

	//========================================
	// ライト

	Transform lightTransform_; // ライトのTransform

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

	// ライトの方向のY軸オフセット
	float lightTime_ = 0.0f;

};
