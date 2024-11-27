#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "ClearScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;

	if (sceneName == "title") {
		newScene = new TitleScene();
		sceneType_ = Title;
	} else if (sceneName == "game") {
		newScene = new GameScene();
		sceneType_ = Game;
	} else if (sceneName == "select") {
		newScene = new SelectScene();
		sceneType_ = Select;
	} else if (sceneName == "clear") {
		newScene = new ClearScene();
		sceneType_ = Clear;
	}

	return newScene;
}

uint32_t SceneFactory::GetSceneType()
{
	uint32_t result = 0;
	switch (sceneType_)
	{
	case Title:
		result = 0;
		break;
	case Game:
		result = 1;
		break;
	case Select:
		result = 2;
		break;
	case Clear:
		result = 3;
		break;
	}

	return result;
}
