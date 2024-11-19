#pragma once
#include <d3d12.h>
#include<wrl.h>
#include <string>
#include <vector>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Mat4x4Func.h"
#include <memory>
#include "Draw2D.h"

//class Model;
class Object3d;

class Boss
{

public: // メンバ関数

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// HPの更新
	void HPUpdate();

	// HPの描画
	void HPDraw();


	// Playerの位置をセットする関数
	void SetPlayerPosition(const Vector3& playerPosition) {
		playerPosition_ = playerPosition;
	}

	// Transformのゲッター
	const Transform& GetTransform() const {
		return transform_;
	}

	// HPのゲッター
	uint32_t GetHP() const { return hp_; }

private: // メンバ変数

	std::unique_ptr<Object3d> object3d_ = nullptr;

	Transform transform_;

	Vector3 playerPosition_;
	const float rotationLerpSpeed_ = 0.05f; // 補間速度

	// HP
	uint32_t hp_ = 1000;

	// ボックスの描画位置とサイズ
	Vector2 boxPosition = Vector2(140.0f, 10.0f); // 画面上の位置
	Vector2 boxSize = Vector2(static_cast<float>(hp_), 25.0f); // ボックスのサイズ
	Vector4 boxColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f); // 赤色

};

