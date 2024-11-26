#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"
#include "Camera.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void TitleScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///
	
	TextureManager::GetInstance()->LoadTexture("uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("ShockWave.png");

	/*titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize("uvChecker.png");
	titleSprite_->SetPos(Vector2(0.0f, 0.0f));*/

	DebugSprite_ = std::make_unique<Sprite>();
	DebugSprite_->Initialize("ShockWave.png");
	DebugSprite_->SetPos({ 0.0f,0.0f });

}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		Object3dBasic::GetInstance()->SetDebug(!Object3dBasic::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif
	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	DebugSprite_->Update();


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("select");
	}
}

void TitleScene::Draw()
{
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



	//------------------------------------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	//タイトルスプライト
	//titleSprite_->Draw();

	DebugSprite_->Draw();

	//--------------------------------------------------//

	///　======================================================　///
	///			  球の描画サンプルコード（消していい）               ///
	/// ======================================================　///
	Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
	//Draw2D::GetInstance()->DrawSphere(Vector3{ 1.0f, 1.0f, 1.0f }, 1.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), viewProjectionMatrix);


	//Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
	Draw2D::GetInstance()->DrawCapsule(1.0f, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 1.001f, 2.0f, 1.001f }, Vector4(1.0f, 1.0f, 1.0f, 1.0f), viewProjectionMatrix);
}

void TitleScene::DrawImGui()
{
#ifdef _DEBUG

	ShowFPS();

#endif // _DEBUG
}
