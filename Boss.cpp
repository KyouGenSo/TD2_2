#include "Boss.h"
#include "Object3d.h"

void Boss::Initialize(){
	object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("Boss.obj");

	//初期位置の設定
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };
}

void Boss::Update(){
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Boss::Draw(){
	object3d_->Draw();
}

Boss::~Boss()
{
	//解放
	delete object3d_;
}
