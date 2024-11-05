#include "GameScene.h"
#include "MyGame.h"
#include "SceneManager.h"
#include"Audio.h"
#include"ModelManager.h"
#include"Object3dBasic.h"
#include"TextureManager.h"
#include"SpriteBasic.h"
#include"Input.h"
#include"ParticleManager.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void GameScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set3D();
#endif
	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	ModelManager::GetInstance()->LoadModel("teapot.obj");

	object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("teapot.obj");

	//---------------------------------------
	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//---------------------------------------
	// 地面の初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

}

void GameScene::Finalize()
{
	delete object3d_;
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1))
	{
		Object3dBasic::GetInstance()->SetDebug(!Object3dBasic::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}

	if (isDebug_)
	{
		DebugCamera::GetInstance()->Update();
	}
#endif
	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	object3d_->Update();

	//---------------------------------------
	// プレイヤーの更新
	player_->Update();

	//---------------------------------------
	// 地面の更新
	ground_->Update();

	// シーン遷移
	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("title");
	}
}

void GameScene::Draw()
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

	// モデル描画
	//object3d_->Draw();

	//---------------------------------------
	// プレイヤーの描画
	player_->Draw();

	//---------------------------------------
	// 地面の描画
	ground_->Draw();

	//-------------------Modelの描画-------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();



	//--------------------------------------------------//
}

void GameScene::DrawImGui()
{
#ifdef _DEBUG

#endif // DEBUG
}
