/*********************************************************************
 * \file   Ground.h
 * \brief  地面クラス
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
///--------------------------------------------------------------
///						 Engine
#include"Sprite.h"
#include"Object3d.h"
#include"Vector3.h"
#include <memory>

class Ground {
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
	// 大きさ
	Vector3 scale_ = Vector3(1.0f, 1.0f, 1.0f);
	// 回転
	Vector3 rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	// 位置
	Vector3 translation_ = Vector3(0.0f, 0.0f, 0.0f);

};

