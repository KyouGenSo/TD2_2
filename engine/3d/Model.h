#pragma once
#include <d3d12.h>
#include<wrl.h>
#include <string>
#include <vector>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Mat4x4Func.h"

class ModelBasic;

class Model
{
public: // 構造体

	// 頂点データ
	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	// マテリアルデータ
	struct MaterialData {
		std::string texturePath;
		uint32_t textureIndex;
	};

	// モデルデータ
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	// マテリアル
	struct Material
	{
		Vector4 color;          // 16 bytes (aligned to 16 bytes)
		bool enableLighting;
		float padding1[3];
		Matrix4x4 uvTransform;
		float shininess;
		bool enableHighlight;
		float padding2[3];
	};
public: // メンバー関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ModelBasic* modelBasic, const std::string& fileName);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	///<summary>
	///objファイルの読み込む
	///	</summary>
	void LoadObjFile(const std::string& directoryPath, const std::string& fileName);

	///<summary>
	///mtlファイルの読み込む
	/// </summary>
	void LoadMtlFile(const std::string& directoryPath, const std::string& fileName);

	// -----------------------------------Setters-----------------------------------//
	void SetShininess(float shininess) { materialData_->shininess = shininess; }
	void SetEnableLighting(bool enableLighting) { materialData_->enableLighting = enableLighting; }
	void SetEnableHighlight(bool enableHighlight) { materialData_->enableHighlight = enableHighlight; }
	void SetAlpha(float alpha) { materialData_->color.w = alpha; }

private: // プライベートメンバー関数

	///<summary>
	///頂点データの生成
	/// </summary>
	void CreateVertexData();

	///<summary>
	///マテリアルデータの生成
	/// </summary>
	void CreateMaterialData();
private:

	ModelBasic* m_modelBasic_;

	std::string directoryFolderName_;

	std::string ModelFolderName_;

	// モデルデータ
	ModelData modelData_;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

};