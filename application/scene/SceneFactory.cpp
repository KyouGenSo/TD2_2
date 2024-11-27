#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "ClearScene.h"
#include "OverScene.h" // OverScene ��ǉ�

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
	else if (sceneName == "over") {
		newScene = new OverScene();
		sceneType_ = Over;
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
	case Over:
		result = 4;
		break;
	}

	return result;
}
