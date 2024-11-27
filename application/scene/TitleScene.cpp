#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object3d.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif
#include <numbers>

void TitleScene::Initialize() {
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	//========================================
	// 平行光源の設定
	Object3dBasic::GetInstance()->SetDirectionalLightIntensity(0.07f);

	//========================================
	// カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();

	//========================================
	// 2Dの初期化
	TextureManager::GetInstance()->LoadTexture("uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("ShockWave.png");

	DebugSprite_ = std::make_unique<Sprite>();
	DebugSprite_->Initialize("ShockWave.png");
	DebugSprite_->SetPos({ 0.0f,0.0f });



	//========================================
	// タイトルモデルの初期化
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Title.obj");
	titleObject_ = std::make_unique<Object3d>();
	titleObject_->Initialize();
	titleObject_->SetModel("Title.obj");

	// スケール、回転、位置の初期設定
	titleTransform_.scale = { 1.0f, 1.0f,1.0f };
	titleTransform_.rotate = { 0.2f, 3.2f, 0.0f };
	titleTransform_.translate = { 0.0f, 5.0f, 0.0f };


	//========================================
	// 地面の初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	//========================================
	// スカイドームの初期化
	ModelManager::GetInstance()->LoadModel("skydome.obj");
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	//========================================
	// ライト
	lightTransform_.translate = { 0.0f, 4.0f, -20.0f };
	// 範囲が狭く光が強いライト設定
	narrowStrongLight_ = {
		{ lightTransform_.translate.x, lightTransform_.translate.y + 2.0f, lightTransform_.translate.z }, // lightPos
		titleTransform_.translate - Vector3(lightTransform_.translate.x, lightTransform_.translate.y + 5.0f, lightTransform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		10.0f, // 光の強さ
		40.0f, // ライト範囲
		0.1f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 5.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 範囲が広く光が弱いライト設定
	wideWeakLight_ = {
		{ lightTransform_.translate.x, lightTransform_.translate.y + 2.0f, lightTransform_.translate.z }, // lightPos
		titleTransform_.translate - Vector3(lightTransform_.translate.x, lightTransform_.translate.y + 2.0f, lightTransform_.translate.z), // lightDir
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // lightColor
		10.0f, // 光の強さ
		50.0f, // ライト範囲
		1.0f, // 光減衰
		std::cos(std::numbers::pi_v<float> / 5.0f), // ライトスポット角度
		true // isSpotLightフラグ
	};

	// 初期ライト設定
	currentLight_ = &narrowStrongLight_;
}

void TitleScene::Finalize() {

}

void TitleScene::Update() {
#ifdef _DEBUG
	if(Input::GetInstance()->TriggerKey(DIK_F1)) {
		Object3dBasic::GetInstance()->SetDebug(!Object3dBasic::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}

	if(isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif
	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	DebugSprite_->Update();

	//========================================
	// カメラの更新
	followCamera_->Update(Vector3{ 0.0f, 0.0f, -10.0f }, Vector3{ 0.0f, 0.0f, 0.0f });

	//========================================
	// タイトルオブジェクトの更新
	//タイトルを上下に揺らす
	titleTransform_.translate.y = 5.0f + std::sin(lightTime_) * 1.5f;
	//傾きも揺らす
	titleTransform_.rotate.x = std::sin(lightTime_) * 0.1f;


	titleObject_->SetScale(titleTransform_.scale);
	titleObject_->SetRotate(titleTransform_.rotate);
	titleObject_->SetTranslate(titleTransform_.translate);
	titleObject_->Update();

	//========================================
	// 地面の更新
	ground_->Update();

	//========================================
	// スカイドームの更新
	skydome_->Update();

	//========================================
	// ライト
	Light();



	if(Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("game");
	}
}

void TitleScene::Draw() {
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();



	//--------------------------------------------------//


	//-------------------Modelの描画-------------------//
	// 3Dモデル共通描画設定
	Object3dBasic::GetInstance()->SetCommonRenderSetting();

	//タイトルオブジェクト
	titleObject_->Draw();

	//地面
	ground_->Draw();

	//スカイドーム
	skydome_->Draw();



	//------------------------------------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	//タイトルスプライト
	//titleSprite_->Draw();

	//DebugSprite_->Draw();

	//--------------------------------------------------//

	///　======================================================　///
	///			  球の描画サンプルコード（消していい）               ///
	/// ======================================================　///
	//Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
	//Draw2D::GetInstance()->DrawSphere(Vector3{ 1.0f, 1.0f, 1.0f }, 1.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), viewProjectionMatrix);


	//Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
	//Draw2D::GetInstance()->DrawCapsule(1.0f, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 1.001f, 2.0f, 1.001f }, Vector4(1.0f, 1.0f, 1.0f, 1.0f), viewProjectionMatrix);
}

void TitleScene::DrawImGui() {
#ifdef _DEBUG

	ImGui::Begin("Player SpotLight");

	ImGui::Text("Current Light Profile: %s", isLightProfileToggled_ ? "Wide Weak" : "Narrow Strong");
	ImGui::DragFloat3("Light Dir", &currentLight_->lightDir.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("Light Pos", &currentLight_->lightPos.x, 0.1f);
	ImGui::ColorEdit4("Light Color", &currentLight_->lightColor.x);
	ImGui::SliderFloat("Light Intensity", &currentLight_->lightIntensity, 0.0f, 10.0f);
	ImGui::SliderFloat("Light Range", &currentLight_->lightRange, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Decay", &currentLight_->lightDecay, 0.0f, 2.0f);
	ImGui::SliderFloat("Light Spot Angle", &currentLight_->lightSpotAngle, 0.0f, 1.0f);
	ImGui::Checkbox("SpotLight", &currentLight_->isSpotLight);

	ImGui::End();

	ShowFPS();

#endif // _DEBUG
}

void TitleScene::Light() {
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

	// 上下移動
	if(Input::GetInstance()->PushKey(DIK_DOWN)) {
		directionVerticalOffset -= 0.02f; // 下方向に移動
	}
	if(Input::GetInstance()->PushKey(DIK_UP)) {
		directionVerticalOffset += 0.02f; // 上方向に移動
	}

	// 左右移動
	if(Input::GetInstance()->PushKey(DIK_LEFT)) {
		directionHorizontalOffset -= 0.02f; // 左方向に回転
	}
	if(Input::GetInstance()->PushKey(DIK_RIGHT)) {
		directionHorizontalOffset += 0.02f; // 右方向に回転
	}

	//ライトの位置修正
	//directionVerticalOffset = std::sin(directionVerticalOffset);
	//directionHorizontalOffset = std::sin(directionHorizontalOffset);

    // ライトが上下左右にジグザグにゆっくり動く
    lightTime_ += 0.008f; 
    directionVerticalOffset = std::sin(lightTime_ * 0.4f) * 0.4f;
    directionHorizontalOffset = std::cos(lightTime_ * 0.2f) * 0.2f;

	// ライトの位置更新
	currentLight_->lightPos = {
		lightTransform_.translate.x,       // ライトのX座標
		lightTransform_.translate.y + 2.0f,// ライトのY座標
		lightTransform_.translate.z        // ライトのZ座標
	};

	// ボスの方向を基準にライトの方向を計算
	Vector3 directionToBoss = titleTransform_.translate - currentLight_->lightPos;

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
