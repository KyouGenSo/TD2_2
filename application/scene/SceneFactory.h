#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public: // sheneType
	enum SceneType
	{
		Title,
		Game,
		Select,
		Clear,
		Over
	};

	SceneType sceneType_;

public: // メンバ関数

	/// <summary>
	/// シーンの生成
	/// </summary>
	BaseScene* CreateScene(const std::string& sceneName) override;

	/// <summary>
	/// sceneTypeのゲッター
	/// </summary>
	uint32_t GetSceneType();
};
