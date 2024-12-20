#pragma once
#include"TakoFramework.h"
#include"Vector2.h"

class MyGame : public TakoFramework
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

private: // メンバ変数

	float bloomThreshold = 0.12f;
	float bloomIntensity = 1.0f;
	float bloomSigma = 3.0f;
	Vector4 fogColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	float fogDensity = 0.002f;

	enum PostEffectType
	{
		NoEffect,
		BloomFog,
	};

	PostEffectType postEffectType;

};