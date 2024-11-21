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
	} else if (sceneName == "game") {
		newScene = new GameScene();
	} else if (sceneName == "select") {
		newScene = new SelectScene();
	} else if (sceneName == "clear") {
		newScene = new ClearScene();
	}

	return newScene;
}
