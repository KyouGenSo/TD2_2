#pragma once
#include "string"

class Boss;

class BossAttackBaseState
{
public: // メンバ関数

	// デストラクタ
	virtual ~BossAttackBaseState();

	// コンストラクタ
	BossAttackBaseState(const std::string& name, Boss* boss) : name_(name), boss_(boss) {};

	virtual void Update() = 0; // 状態ごとの更新処理

protected: // メンバ変数

	//状態名
	std::string name_;
	//操作対象の敵
	Boss* boss_ = nullptr;

};

