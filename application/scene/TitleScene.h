#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include <vector>
#include"Vector2.h"
#include <memory>
#include "imgui.h"

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
};
