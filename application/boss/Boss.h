#pragma once
#include <memory>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Mat4x4Func.h"
#include "Transform.h"
#include "Draw2D.h"
#include "BossAttackBaseState.h"
#include "BossNuclear.h"
#include "ObjectBase.h"

class Object3d;

class Boss : public ObjectBase {
public: // メンバ関数

	void DecreaseHP(uint32_t damage) {
		if (hp_ > damage) {
			hp_ -= damage;
		} else {
			hp_ = 0; // HPが0未満にならないようにする
		}
	}

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 通常の動き
	void Move();

	// 描画
	void Draw();

	// ImGui描画
	void DrawImGui();

	// HP描画
	void HPUpdate();

	// HP更新
	void HPDraw();

	// ステート変更
	void ChangeState(std::unique_ptr<BossAttackBaseState> state);

	// Player位置をセット
	void SetPlayerPosition(const Vector3& playerPosition) {
		playerPosition_ = playerPosition;
	}

	void SetTransform(const Transform& transform) { transform_ = transform; }

	Transform& GetTransform() { return transform_; }
	uint32_t GetHP() const { return hp_; }

	//コアを取得
	std::vector<std::unique_ptr<BossNuclear>>& GetCores();

	/// @brief 衝突処理イベント
	/// @param objectBase 衝突したオブジェクト
	void OnCollision(ObjectBase* objectBase) override;

	// 可視性を設定するメソッド
	void SetVisible(bool visible) { isVisible_ = visible; }

	// 可視性を取得するメソッド
	bool IsVisible() const { return isVisible_; }

private:// メンバ変数

	std::unique_ptr<Object3d> object3d_ = nullptr;
	Transform transform_;
	Vector3 playerPosition_;
	uint32_t hp_ = 1000;

	Vector2 boxPosition = { 140.0f, 10.0f };
	Vector2 boxSize = { 1000.0f, 25.0f };
	Vector4 boxColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	// 現在の状態
	std::unique_ptr<BossAttackBaseState> state_;

	std::vector<std::unique_ptr<BossNuclear>> cores_;  // 核のリスト

	float lightHitDuration_ = 0.0f; // ライトが当たった時間を追跡
	bool coresAreVisible_ = false; // 核が可視化されているかどうか
	bool isLightHitting_ = false; // ライトが現在当たっているかどうか


private: // メンバ関数

	// イージング関数 (Ease-In-Out)
	float EaseInOut(float t) {
		if (t < 0.5f) {
			return 2.0f * t * t; // 前半: 加速
		} else {
			return 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f; // 後半: 減速
		}
	}

	// イージング関数 (Ease-In-Expo)
	float EaseInExpo(float t) {
		return t == 0.0f ? 0.0f : pow(2.0f, 10.0f * (t - 1.0f)); // 最初はゆっくり、途中から急激に加速
	}

	// イージング関数 (Ease-Out-Bounce)
	float EaseOutBounce(float t) {
		if (t < 1.0f / 2.75f) {
			return 7.5625f * t * t;
		} else if (t < 2.0f / 2.75f) {
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		} else if (t < 2.5f / 2.75f) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		} else {
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	}


	//行動フェーズ
	enum class Phase {
		Usually,  //　通常状態
		Down, // ダウン状態
		GettingUp,  // 起き上がり状態
	};

	Phase phase_ = Phase::Usually;

	// 各状態に対応するメンバ関数
	void Usually();
	void Down();
	void GettingUp();

	// 状態のメンバ関数ポインタのテーブル
	static void (Boss::* spFuncTable[])();

	bool isVisible_ = false; // 核の可視性フラグ

};
