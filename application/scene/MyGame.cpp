#include "MyGame.h"
#include"Audio.h"
#include"Input.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "Draw2D.h"
#include "Object3dBasic.h"
#include "PostEffect.h"

void MyGame::Initialize()
{

	TakoFramework::Initialize();

#pragma region 汎用機能初期化-------------------------------------------------------------------------------------------------------------------
	// 入力クラスの初期化
	Input::GetInstance()->Initialize(winApp_);

	// オーディオの初期化
	Audio::GetInstance()->Initialize("resources/Sound/");

#pragma endregion

	// シーンの初期化
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("game", 0.0f);

	postEffectType = NoEffect;
	PostEffect::GetInstance()->SetBloomIntensity(bloomIntensity);
	PostEffect::GetInstance()->SetBloomThreshold(bloomThreshold);
	PostEffect::GetInstance()->SetBloomSigma(bloomSigma);
	PostEffect::GetInstance()->SetFogColor(fogColor);
	PostEffect::GetInstance()->SetFogDensity(fogDensity);

}




void MyGame::Finalize()
{
	TakoFramework::Finalize();

	// Audioの解放
	Audio::GetInstance()->Finalize();

	// 入力クラスの解放
	Input::GetInstance()->Finalize();
}

void MyGame::Update()
{
	// カメラの更新
	defaultCamera_->Update();

	// 入力情報の更新
	Input::GetInstance()->Update();

	TakoFramework::Update();

	//　サウンドの更新
	Audio::GetInstance()->Update();

	uint32_t sceneIndex = SceneManager::GetInstance()->GetSceneIndex();
	if (sceneIndex == 1)
	{
		postEffectType = BloomFog;
	} else
	{
		postEffectType = NoEffect;
	}

}

void MyGame::Draw()
{
	/// ============================================= ///
	/// ------------------シーン描画-------------------///
	/// ============================================= ///

	// 描画前の処理(レンダーテクスチャを描画対象に設定)
	dx12_->SetRenderTexture();

	// テクスチャ用のsrvヒープの設定
	SrvManager::GetInstance()->BeginDraw();

	// シーンの描画
	SceneManager::GetInstance()->Draw();

	Draw2D::GetInstance()->Reset();


	/// ===================================================== ///
	/// ------------------ポストエフェクト描画-------------------///
	/// ===================================================== ///
	// SwapChainを描画対象に設定
	dx12_->SetSwapChain();

	// PostEffectの描画
	switch (postEffectType)
	{
	case::MyGame::NoEffect:
		PostEffect::GetInstance()->Draw("NoEffect");
		break;
	case::MyGame::BloomFog:
		PostEffect::GetInstance()->Draw("BloomFog");
		break;
	}


	/// ========================================= ///
	///-------------------ImGui-------------------///
	/// ========================================= ///
#ifdef _DEBUG

	imguiManager_->Begin();

	SceneManager::GetInstance()->DrawImGui();

	Draw2D::GetInstance()->ImGui();

	// PostEffectのパラメータ調整
	ImGui::Begin("PostEffect");
	if (ImGui::BeginTabBar("PostEffectTab"))
	{

		if (ImGui::BeginTabItem("PostEffectType"))
		{
			ImGui::RadioButton("NoEffect", (int*)&postEffectType, NoEffect);
			ImGui::RadioButton("BloomFog", (int*)&postEffectType, BloomFog);

			ImGui::EndTabItem();
		}

		//ImGui::Separator();
		if (ImGui::BeginTabItem("PostEffect"))
		{

			if (postEffectType == BloomFog)
			{
				ImGui::DragFloat("BloomIntensity", &bloomIntensity, 0.01f, 0.0f, 10.0f);
				PostEffect::GetInstance()->SetBloomIntensity(bloomIntensity);
				ImGui::DragFloat("BloomThreshold", &bloomThreshold, 0.01f, 0.0f, 1.0f);
				PostEffect::GetInstance()->SetBloomThreshold(bloomThreshold);
				ImGui::DragFloat("BloomSigma", &bloomSigma, 0.01f, 0.0f, 10.0f);
				PostEffect::GetInstance()->SetBloomSigma(bloomSigma);
				ImGui::ColorEdit4("FogColor", &fogColor.x);
				PostEffect::GetInstance()->SetFogColor(fogColor);
				ImGui::DragFloat("FogDensity", &fogDensity, 0.001f, 0.0f, 1.0f);
				PostEffect::GetInstance()->SetFogDensity(fogDensity);
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}


	ImGui::End();

	imguiManager_->End();

	//imguiの描画
	imguiManager_->Draw();
#endif


	// 描画後の処理
	dx12_->EndDraw();
}
