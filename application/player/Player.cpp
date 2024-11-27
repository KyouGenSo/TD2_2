#include "Player.h"
#include "Input.h"
#include "ModelManager.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#endif // _DEBUG
#include <cmath>
#include <numbers>
#include <random>
#include "BossBullet.h"
#include "Draw2D.h"
#include <SceneManager.h>

void Player::Initialize(Boss* boss) {
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Player.obj");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");

	// スケール、回転、位置の初期設定
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	boss_ = boss; // Boss のポインタを設定

	followCamera_ = std::make_unique<FollowCamera>();

	//// lightの初期設定-----------------------------------------------------------------------------------------------------------------------------

	// 範囲が狭く光が強いライト設定
	narrowStrongLight_ = {
		{ transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z }, // lightPos
		boss->GetTransform().translate - Vector3(transform_.translate.x, transform_.translate.y + 5.0f, transform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		35.0f, // 光の強さ
		30.0f, // ライト範囲
		0.1f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 20.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 範囲が広く光が弱いライト設定
	wideWeakLight_ = {
		{ transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z }, // lightPos
		boss->GetTransform().translate - Vector3(transform_.translate.x, transform_.translate.y + 2.0f, transform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		20.0f, // 光の強さ
		50.0f, // ライト範囲
		1.0f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 5.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 初期ライト設定
	currentLight_ = &narrowStrongLight_;

	// ライトの位置と方向を常に更新
	//currentLight_->lightPos = { transform_.translate.x, currentLight_->lightPos.y, transform_.translate.z };
	//currentLight_->lightDir = (boss_->GetTransform().translate - currentLight_->lightPos).normalize();

	////-------------------------------------------------------------------------------------------------------------------------------------------


	actionDelay_ = 180; // 初期の制限時間を設定
	canAct_ = false;   // 初期状態では行動不可

	//========================================
	// ライト用当たり判定の初期化
	lightCollision_ = std::make_unique<LightCollision>();
	lightCollision_->Initialize();

	//========================================
	// プレイヤーの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);

}

void Player::Update() {
	// ボスが存在しない場合、処理をスキップ
	if(boss_ == nullptr) return;

	//ライト
	Light();


	// 移動処理
	Move();


	UpdateHP();

	if (hp_ <= 0)
	{
		SceneManager::GetInstance()->ChangeScene("over");
	}

	// 砂埃パーティクルの更新
	for(auto it = dustParticles_.begin(); it != dustParticles_.end(); ) {
		DustParticle& particle = *it;

		// パーティクルの位置を更新（Y軸の移動なし）
		Vector3 position = particle.object->GetTranslate();
		position.x += particle.velocity.x;
		position.z += particle.velocity.z;

		particle.object->SetTranslate(position);
		particle.object->Update();

		// 寿命を減らし、寿命切れのパーティクルを削除
		particle.lifetime--;
		if(particle.lifetime <= 0) {
			it = dustParticles_.erase(it);
		} else {
			++it;
		}
	}



	// BossにPlayerの位置を通知
	boss_->SetPlayerPosition(transform_.translate);

	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	// 追従カメラ
	followCamera_->Update(transform_.translate, transform_.rotate);


	//========================================
	// 判定場所の処理
	//プレイヤー
	Vector3 endPos = transform_.translate + Vector3(0.0f, 1.0f, 0.0f);
	ObjectBase::Update(transform_.translate, endPos);

	// ライト判定の開始位置を設定
	// ライトの現在位置 (currentLight_->lightPos) を判定のスタート地点として指定する
	lightCollision_->SetStart(currentLight_->lightPos);

	// ライト判定の終了位置を設定
	// 現在のライトの位置から、ライトが向いている方向 (currentLight_->lightDir) に
	// 射程範囲 (currentLight_->lightRange) だけ延ばした位置を終了地点として指定する
	lightCollision_->SetEnd(currentLight_->lightPos + currentLight_->lightDir * currentLight_->lightRange);


	// ライト判定を更新
	// 上記で設定した開始位置と終了位置に基づいて判定の処理を再計算する
	lightCollision_->Update();


	////ライトの判定
	//// NOTE:ここではライトの開始位置をプレイヤーの位置に設定している
	//lightCollision_->SetStart(transform_.translate);
	//// ライトの終了位置を設定
	//// NOTE:endPosは仮の値なので、実際にはプレイヤーの位置からライトの方向に伸ばした先の位置を設定する
	//endPos = transform_.translate + Vector3(0.0f, 0.0f, 10.0f);
	//lightCollision_->SetEnd(endPos);
	//// ライトの更新
	//// NOTE:更新前には必ず開始位置と終了位置を設定する
	//lightCollision_->Update();
}


void Player::Draw() {
	// モデルの描画
	object3d_->Draw();

	// 砂埃パーティクルの描画
	for(const auto& particle : dustParticles_) {
		particle.object->Draw();
	}
}

void Player::Move() {
#ifdef _DEBUG
	// コントローラーの状態を取得
	ZeroMemory(&controllerState_, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &controllerState_);

	// コントローラーが接続されている場合、入力を処理
	if(dwResult == ERROR_SUCCESS) {
		// 左スティックの入力値を取得
		float LX = controllerState_.Gamepad.sThumbLX;
		float LY = controllerState_.Gamepad.sThumbLY;

		// デッドゾーンの設定
		const float DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		if(abs(LX) < DEADZONE) LX = 0;
		if(abs(LY) < DEADZONE) LY = 0;

		// スティックの入力がある場合に移動
		if(LX != 0 || LY != 0) {
			// スティックの値を正規化
			float magnitude = sqrtf(LX * LX + LY * LY);
			float normalizedLX = LX / magnitude;
			// 未使用のため削除: float normalizedLY = LY / magnitude;

			// 移動速度の計算
			float moveSpeed = rotationSpeed_ * ( magnitude / 32767.0f ); // 32767 はスティックの最大値

			// プレイヤーの回転角度を更新
			angle_ += normalizedLX * moveSpeed;

			// ボスの位置を中心に円状に移動
			const Transform& bossTransform = boss_->GetTransform();
			transform_.translate.x = bossTransform.translate.x + radius_ * cosf(angle_);
			transform_.translate.z = bossTransform.translate.z + radius_ * sinf(angle_);

			// ボスの方向を向くための角度計算
			Vector3 directionToBoss = bossTransform.translate - transform_.translate;
			transform_.rotate.y = atan2f(directionToBoss.x, directionToBoss.z);

			GenerateDust();
		}

		// ジャンプ処理
		if(( controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_A ) && !isJumping_) {
			isJumping_ = true;
			jumpVelocity_ = jumpPower_;
			GenerateDust();
		}

		if(isJumping_) {
			transform_.translate.y += jumpVelocity_;
			jumpVelocity_ += gravity_;

			if(transform_.translate.y <= 0.0f) {
				transform_.translate.y = 0.0f;
				isJumping_ = false;
				jumpVelocity_ = 0.0f;
				GenerateDust();
			}
		}
	}
#endif // _DEBUG

	// プレイヤーの左右移動 (Boss の周りを回転)
	if(Input::GetInstance()->PushKey(DIK_A)) {
		angle_ -= rotationSpeed_; // 左回転
		GenerateDust();
	}
	if(Input::GetInstance()->PushKey(DIK_D)) {
		angle_ += rotationSpeed_; // 右回転
		GenerateDust();
	}

	// Boss の位置を中心に円状に移動
	const Transform& bossTransform = boss_->GetTransform();
	transform_.translate.x = bossTransform.translate.x + radius_ * cosf(angle_);
	transform_.translate.z = bossTransform.translate.z + radius_ * sinf(angle_);

	// Boss の方向を向くための角度計算
	Vector3 directionToBoss = bossTransform.translate - transform_.translate;
	transform_.rotate.y = atan2f(directionToBoss.x, directionToBoss.z);

	// ジャンプ処理
	if(Input::GetInstance()->PushKey(DIK_W) && !isJumping_) {
		isJumping_ = true;
		jumpVelocity_ = jumpPower_;
		GenerateDust();
	}

	if(isJumping_) {
		transform_.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		if(transform_.translate.y <= 0.0f) {
			transform_.translate.y = 0.0f;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
			GenerateDust();
		}
	}

}

void Player::Light() {
	static float directionVerticalOffset = 0.0f;  // ライトの方向のY軸オフセット
	static float directionHorizontalOffset = 0.0f; // ライトの方向のX軸オフセット

	// ライト切り替え
	if(Input::GetInstance()->TriggerKey(DIK_L)) {
		if(isLightProfileToggled_) {
			currentLight_ = &narrowStrongLight_;
		} else {
			currentLight_ = &wideWeakLight_;
		}
		isLightProfileToggled_ = !isLightProfileToggled_;
	}

	// オフセットの制限値
	const float maxVerticalOffset = 1.0f; // 上下の制限
	const float minVerticalOffset = -1.0f;
	const float maxHorizontalOffset = 1.0f; // 左右の制限
	const float minHorizontalOffset = -1.0f;

	//コントローラでのライトの方向の変更
	// 右スティックの入力値を取得
	float RX = controllerState_.Gamepad.sThumbRX;
	float RY = controllerState_.Gamepad.sThumbRY;

	// デッドゾーンの設定
	const float DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	if(abs(RX) < DEADZONE) RX = 0;
	if(abs(RY) < DEADZONE) RY = 0;

	// スティックの入力がある場合に移動
	if(RX != 0 || RY != 0) {
		// スティックの値を正規化
		float magnitude = sqrtf(RX * RX + RY * RY);
		float normalizedRX = RX / magnitude;
		float normalizedRY = RY / magnitude;

		// 移動速度の計算
		float moveSpeed = 0.02f * ( magnitude / 32767.0f ); // 32767 はスティックの最大値

		// ライトの方向オフセットを更新
		directionHorizontalOffset += normalizedRX * moveSpeed;
		directionVerticalOffset += normalizedRY * moveSpeed;

		// ライトの方向オフセットを制限
		directionVerticalOffset = std::clamp(directionVerticalOffset, minVerticalOffset, maxVerticalOffset);
		directionHorizontalOffset = std::clamp(directionHorizontalOffset, minHorizontalOffset, maxHorizontalOffset);
	}

	//ライトの向きの変更
	// ライトの方向オフセットを更新
	if(Input::GetInstance()->PushKey(DIK_UP)) {
		directionVerticalOffset += 0.01f;
	}
	if(Input::GetInstance()->PushKey(DIK_DOWN)) {
		directionVerticalOffset -= 0.01f;
	}
	if(Input::GetInstance()->PushKey(DIK_LEFT)) {
		directionHorizontalOffset -= 0.01f;
	}
	if(Input::GetInstance()->PushKey(DIK_RIGHT)) {
		directionHorizontalOffset += 0.01f;
	}


	// 制限を適用
	directionVerticalOffset = std::clamp(directionVerticalOffset, minVerticalOffset, maxVerticalOffset);
	directionHorizontalOffset = std::clamp(directionHorizontalOffset, minHorizontalOffset, maxHorizontalOffset);

	// ライトの位置更新
	currentLight_->lightPos = {
		transform_.translate.x,       // プレイヤーのX座標
		transform_.translate.y + 2.0f,// プレイヤーのY座標
		transform_.translate.z        // プレイヤーのZ座標
	};

	// ボスの方向を基準にライトの方向を計算
	Vector3 directionToBoss = {
	boss_->GetTransform().translate.x - currentLight_->lightPos.x
		, currentLight_->lightPos.y
		, boss_->GetTransform().translate.z - currentLight_->lightPos.z };

	// ライトの方向をリセットし、プレイヤーからボスへ向かう方向を初期値とする
	Vector3 initialDirection = directionToBoss.normalize();

	// 各オフセットを適用する
	// Y軸回転による左右方向のオフセットを回転行列で適用
	Matrix4x4 rotationMatrix = Mat4x4::MakeRotateY(directionHorizontalOffset);
	Vector3 rotatedDirection = Mat4x4::TransForm(rotationMatrix, initialDirection);

	// 垂直方向のオフセットを適用
	rotatedDirection.y += directionVerticalOffset;

	// 正規化された方向を計算して設定
	currentLight_->lightDir = rotatedDirection.normalize();

	// スポットライトの更新
	Object3dBasic::GetInstance()->SetSpotLight(
		currentLight_->lightPos,
		currentLight_->lightDir,
		currentLight_->lightColor,
		currentLight_->lightIntensity,
		currentLight_->lightRange,
		currentLight_->lightDecay,
		currentLight_->lightSpotAngle,
		currentLight_->isSpotLight
	);
}



void Player::DrawImGui() {
#ifdef _DEBUG
	ImGui::Begin("Player SpotLight");

	ImGui::Text("Current Light Profile: %s", isLightProfileToggled_ ? "Wide Weak" : "Narrow Strong");
	ImGui::DragFloat3("Light Dir", &currentLight_->lightDir.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("Light Pos", &currentLight_->lightPos.x, 0.1f);
	ImGui::ColorEdit4("Light Color", &currentLight_->lightColor.x);
	ImGui::SliderFloat("Light Intensity", &currentLight_->lightIntensity, 0.0f, 50.0f);
	ImGui::SliderFloat("Light Range", &currentLight_->lightRange, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Decay", &currentLight_->lightDecay, 0.0f, 2.0f);
	ImGui::SliderFloat("Light Spot Angle", &currentLight_->lightSpotAngle, 0.0f, 1.0f);
	ImGui::Checkbox("SpotLight", &currentLight_->isSpotLight);

	ImGui::End();
#endif // DEBUG
}

///=============================================================================
///						 衝突判定イベント
void Player::OnCollision(ObjectBase* objectBase) {
	//Bossとの衝突判定
	if(dynamic_cast<Boss*>( objectBase ) != nullptr) {
		//赤色に変更
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//ボスの弾との接触
	if(dynamic_cast<BossBullet*>( objectBase ) != nullptr) {
		//赤色に変更
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		hp_ -= 20;
	}
}

void Player::GenerateDust() {
	// ランダムな速度を生成
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(-0.1f, 0.1f); // 水平方向のランダムな速度
	std::uniform_real_distribution<float> distZ(-0.1f, 0.1f);

	Vector3 position = transform_.translate; // プレイヤーの足元
	position.y = 0.1f;

	auto particleObject = std::make_unique<Object3d>();
	particleObject->Initialize();
	particleObject->SetModel("ShockWave.obj");
	particleObject->SetTranslate(position);
	particleObject->SetScale({ 0.3f, 0.2f, 0.2f });
	particleObject->Update();

	// Y軸方向の速度を 0 に設定
	Vector3 velocity = { distX(gen), 0.0f, distZ(gen) };
	DustParticle particle = { std::move(particleObject), velocity, 30 }; // 寿命30フレーム
	dustParticles_.emplace_back(std::move(particle));
}

void Player::DrawHPBar()
{
	// 背景用バー（灰色）
	Draw2D::GetInstance()->DrawBox(hpBarPosition_, { 20.0f, 200.0f }, { 0.5f, 0.5f, 0.5f, 0.5f });

	// 現在の HP バー
	Vector2 currentBarPosition = { hpBarPosition_.x, hpBarPosition_.y + (200.0f - hpBarSize_.y) };
	Draw2D::GetInstance()->DrawBox(currentBarPosition, hpBarSize_, hpBarColor_);

	// 枠線の描画
	Vector4 borderColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector2 topRight = { hpBarPosition_.x + 20.0f, hpBarPosition_.y };
	Vector2 bottomLeft = { hpBarPosition_.x, hpBarPosition_.y + 200.0f };
	Vector2 bottomRight = { hpBarPosition_.x + 20.0f, hpBarPosition_.y + 200.0f };
	Draw2D::GetInstance()->DrawLine(hpBarPosition_, topRight, borderColor);       // 上辺
	Draw2D::GetInstance()->DrawLine(topRight, bottomRight, borderColor);       // 右辺
	Draw2D::GetInstance()->DrawLine(bottomRight, bottomLeft, borderColor);     // 下辺
	Draw2D::GetInstance()->DrawLine(bottomLeft, hpBarPosition_, borderColor);    // 左辺
}

void Player::UpdateHP()
{
	// HP の割合を計算
	float hpRatio = static_cast<float>(hp_) / maxHp_;

	// HP バーの高さを更新
	hpBarSize_.y = 200.0f * hpRatio; // 最大高さは 200

	// HP バーの色を変更
	if (hpRatio > 0.5f) {
		hpBarColor_ = { 0.0f, 1.0f, 0.0f, 1.0f }; // 緑
	}
	else if (hpRatio > 0.2f) {
		hpBarColor_ = { 1.0f, 1.0f, 0.0f, 1.0f }; // 黄色
	}
	else {
		hpBarColor_ = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤
	}
}


