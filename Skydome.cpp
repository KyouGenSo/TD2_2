#include "Skydome.h"
#include "Object3d.h"

void Skydome::Initialize(){
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("skydome.obj");

	//初期位置の設定
	transform_.scale = { 60.0f,60.0f,60.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };
}

void Skydome::Update(){
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Skydome::Draw(){
	object3d_->Draw();
}
