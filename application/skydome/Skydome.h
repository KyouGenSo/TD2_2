#pragma once
#include <d3d12.h>
#include<wrl.h>
#include <string>
#include <vector>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Mat4x4Func.h"
#include "Transform.h"
#include <memory>

class Object3d;

class Skydome
{

public:
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr<Object3d> object3d_ = nullptr;

	Transform transform_;

};

