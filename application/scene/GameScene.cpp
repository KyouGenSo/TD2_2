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
#include"BossBullet.h"
#include"Boss.h"
#include"AttackPhase3.h"
#include"AttackPhase4.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void GameScene::Initialize() {
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
#endif
	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	// 平行光源の設定
	Object3dBasic::GetInstance()->SetDirectionalLightIntensity(0.18f);

	ModelManager::GetInstance()->LoadModel("player.obj");
	ModelManager::GetInstance()->LoadModel("Boss.obj");
	ModelManager::GetInstance()->LoadModel("skydome.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("Title.obj");
	ModelManager::GetInstance()->LoadModel("Nuclear.obj");
	ModelManager::GetInstance()->LoadModel("ShockWave.obj");
	ModelManager::GetInstance()->LoadModel("Meteor.obj");
	ModelManager::GetInstance()->LoadModel("Prediction.obj");
	ModelManager::GetInstance()->LoadModel("Particle.obj");
	TextureManager::GetInstance()->LoadTexture("Nuclear.png");
	TextureManager::GetInstance()->LoadTexture("ShockWave.png");
	TextureManager::GetInstance()->LoadTexture("circle.png");

	//---------------------------------------
	// ボスの初期化
	boss_ = new Boss();
	boss_->Initialize();

	//---------------------------------------
	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(boss_);

	//---------------------------------------
	// 地面の初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	//---------------------------------------
	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	//---------------------------------------
	// コリジョンマネージャの初期化
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();


}

void GameScene::Finalize() {
	delete boss_;
}

void GameScene::Update() {
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

	//---------------------------------------
	// ボスの更新
	boss_->Update();

	//---------------------------------------
	// プレイヤーの更新
	player_->Update();

	//---------------------------------------
	// 地面の更新
	ground_->Update();

	//---------------------------------------
	// 天球の更新
	skydome_->Update();

	//---------------------------------------
	// コリジョンマネージャの処理
	//リセット
	collisionManager_->Reset();
	//追加
	collisionManager_->AddCollider(player_.get());//プレイヤー
	//プレイヤーのライトの当たり判定の追加
	collisionManager_->AddCollider(player_->GetLightCollision());
	//ボス
	collisionManager_->AddCollider(boss_);
	//ボスの弾の当たり判定の追加
	//BossAttackBaseState* state = boss_->GetCurrentState();
	// AttackPhase3の弾を取得して追加
	if(auto attackPhase3 = dynamic_cast<AttackPhase3*>( boss_->GetCurrentState() )) {
		const std::list<BossBullet>& bullets = attackPhase3->GetBullets();
		for(auto& bullet : bullets) {
			collisionManager_->AddCollider(const_cast<BossBullet*>(&bullet));
		}
	}
	// AttackPhase4の隕石の当たり判定を追加
	if(auto attackPhase4 = dynamic_cast<AttackPhase4*>( boss_->GetCurrentState() )) {
		if(auto meteorCollision = attackPhase4->GetMeteorCollision()) {
			collisionManager_->AddCollider(meteorCollision);
		}
	}

	// ボスのコアの追加
	std::vector<std::unique_ptr<BossNuclear>>& cores = boss_->GetCores();
	for(auto& core : cores) {
		if(core) {
			collisionManager_->AddCollider(core.get()); // unique_ptrから生のポインタを取得
		}
	}

	//すべての当たり判定をチェック
	collisionManager_->CheckAllCollisions();
	//更新
	collisionManager_->Update();

	// シーン遷移
	if(Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("clear");
	}
}

void GameScene::Draw() {
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

	//---------------------------------------
	// ボスの描画
	boss_->Draw();

	//---------------------------------------
	// プレイヤーの描画
	player_->Draw();

	//---------------------------------------
	// 地面の描画
	ground_->Draw();

	//---------------------------------------
	// 天球の描画
	skydome_->Draw();


	//-------------------Modelの描画-------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	//---------------------------------------
	// コリジョンマネージャの描画
	collisionManager_->Draw();


	// ボスのHPバーの描画
	boss_->HPDraw();

	player_->DrawHPBar();


	//--------------------------------------------------//
}

void GameScene::DrawImGui() {
#ifdef _DEBUG

	player_->DrawImGui();

	boss_->DrawImGui();

	collisionManager_->DrawImGui();

	ShowFPS();

#endif // DEBUG
}
