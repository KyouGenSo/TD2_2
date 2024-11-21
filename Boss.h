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
#include "ObjectBase.h"

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

	// Playerの位置をセットする関数
	void SetPlayerPosition(const Vector3& playerPosition) {
		playerPosition_ = playerPosition;
	}

	// Transformのゲッター
	const Transform& GetTransform() const {
		return transform_;
	}

private: // メンバ変数

	std::unique_ptr<Object3d> object3d_ = nullptr;

	Transform transform_;

	Vector3 playerPosition_;
	const float rotationLerpSpeed_ = 0.05f; // 補間速度
};

