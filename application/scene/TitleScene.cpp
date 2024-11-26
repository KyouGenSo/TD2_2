#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void TitleScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();
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
		Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
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

	Draw2D::GetInstance()->DrawBox(Vector2(500.0f, 500.0f), Vector2(100.0f, 100.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));

}

void TitleScene::DrawImGui()
{
#ifdef _DEBUG

	ShowFPS();

#endif // _DEBUG
}
