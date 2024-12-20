#include "PostEffect.h"
#include "DX12Basic.h"
#include "SrvManager.h"
#include "Logger.h"
#include "StringUtility.h"
#include "Object3dbasic.h"
#include "Camera.h"

PostEffect* PostEffect::instance_ = nullptr;

PostEffect* PostEffect::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new PostEffect();
	}
	return instance_;
}

void PostEffect::Initialize(DX12Basic* dx12)
{
	m_dx12_ = dx12;

	InitRenderTexture();

	CreateDepthBufferSRV();

	CreatePSO("NoEffect");

	CreatePSO("BloomFog");

	CreateBloomParam();

	CreateFogParam();

	CreateCameraForGPU();
}

void PostEffect::Finalize()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void PostEffect::BeginDraw()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dx12_->GetDSVHeapHandleStart();

	// 描画先のRTVを設定
	m_dx12_->GetCommandList()->OMSetRenderTargets(1, &renderTextureRTVHandleA_, false, &dsvHandle);

	float clearColor[] = { kRenderTextureClearColor_.x, kRenderTextureClearColor_.y, kRenderTextureClearColor_.z, kRenderTextureClearColor_.w };

	// 画面の色をクリア
	m_dx12_->GetCommandList()->ClearRenderTargetView(renderTextureRTVHandleA_, clearColor, 0, nullptr);
}

void PostEffect::Draw(const std::string& effectName)
{
	// ルートシグネチャの設定
	m_dx12_->GetCommandList()->SetGraphicsRootSignature(rootSignatures_[effectName].Get());

	// パイプラインステートの設定
	m_dx12_->GetCommandList()->SetPipelineState(pipelineStates_[effectName].Get());

	// トポロジの設定
	m_dx12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファの設定
	SetParamResource(effectName);

	SrvManager::GetInstance()->SetRootDescriptorTable(0, rtvSrvIndex_);
	SrvManager::GetInstance()->SetRootDescriptorTable(4, dsvSrvIndex_);

	// 描画
	m_dx12_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}

void PostEffect::SetBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = renderTextureResourceA_.Get();

	barrier.Transition.StateBefore = stateBefore;
	barrier.Transition.StateAfter = stateAfter;

	m_dx12_->GetCommandList()->ResourceBarrier(1, &barrier);
}

void PostEffect::SetBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, ID3D12Resource* resource)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource;
	barrier.Transition.StateBefore = stateBefore;
	barrier.Transition.StateAfter = stateAfter;

	m_dx12_->GetCommandList()->ResourceBarrier(1, &barrier);
}

void PostEffect::SetBloomThreshold(float threshold)
{
	bloomParam_->threshold = threshold;
}

void PostEffect::SetBloomIntensity(float intensity)
{
	bloomParam_->intensity = intensity;
}

void PostEffect::SetBloomSigma(float sigma)
{
	bloomParam_->sigma = sigma;
}

void PostEffect::SetFogColor(const Vector4& color)
{
	fogParam_->color = color;
}

void PostEffect::SetFogDensity(float density)
{
	fogParam_->density = density;
}

void PostEffect::InitRenderTexture()
{
	// レンダーテクスチャリソースの生成
	m_dx12_->CreateRenderTextureResource(renderTextureResourceA_, WinApp::kClientWidth, WinApp::kClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTextureClearColor_);
	renderTextureResourceA_->SetName(L"PostEffectRenderTexture");

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャとして書き込む

	renderTextureRTVHandleA_ = m_dx12_->GetRenderTextureRTVHandle();

	// RTVの生成
	m_dx12_->GetDevice()->CreateRenderTargetView(renderTextureResourceA_.Get(), &rtvDesc, renderTextureRTVHandleA_);

	rtvSrvIndex_ = SrvManager::GetInstance()->Allocate();

	// SRVの生成
	SrvManager::GetInstance()->CreateSRVForTexture2D(rtvSrvIndex_, renderTextureResourceA_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
}

void PostEffect::CreateDepthBufferSRV()
{
	// SRVの生成
	dsvSrvIndex_ = SrvManager::GetInstance()->Allocate();

	SrvManager::GetInstance()->CreateSRVForTexture2D(dsvSrvIndex_, m_dx12_->GetDepthStencilResource(), DXGI_FORMAT_R32_FLOAT, 1);
}

void PostEffect::CreateRootSignature(const std::string& effectName)
{
	HRESULT hr;

	// rootSignatureの生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // テクスチャの補間方法
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // テクスチャの繰り返し方法
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // テクスチャの繰り返し方法
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // テクスチャの繰り返し方法
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX; // ミップマップの最大LOD
	samplerDesc[0].ShaderRegister = 0; // レジスタ番号
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	descriptionRootSignature.pStaticSamplers = samplerDesc;
	descriptionRootSignature.NumStaticSamplers = _countof(samplerDesc);

	// DescriptorRangeの設定。
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // レジスタ番号
	descriptorRange[0].NumDescriptors = 1; // ディスクリプタ数
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange2[1] = {};
	descriptorRange2[0].BaseShaderRegister = 1; // レジスタ番号
	descriptorRange2[0].NumDescriptors = 1; // ディスクリプタ数
	descriptorRange2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameterの設定。複数設定できるので配列
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// Texture
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ディスクリプタテーブルを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // ディスクリプタレンジを設定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // レンジの数

	// Param
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 定数バッファビューを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号とバインド

	// Param
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 定数バッファビューを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	rootParameters[2].Descriptor.ShaderRegister = 1; // レジスタ番号とバインド

	// Param
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 定数バッファビューを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	rootParameters[3].Descriptor.ShaderRegister = 2; // レジスタ番号とバインド

	// 深度バッファテクスチャ
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ディスクリプタテーブルを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = descriptorRange2; // ディスクリプタレンジを設定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange2); // レンジの数

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = m_dx12_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignatures_[effectName].GetAddressOf()));
	signatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignatures_[effectName].GetAddressOf());
	assert(SUCCEEDED(hr));
}

void PostEffect::CreatePSO(const std::string& effectName)
{
	HRESULT hr;

	// RootSignatureの生成
	CreateRootSignature(effectName);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	// BlendState
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	// 裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	// shaderのコンパイル
	std::wstring psPath = L"resources/shaders/" + StringUtility::ConvertString(effectName) + L".PS.hlsl";

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = m_dx12_->CompileShader(L"resources/shaders/FullScreen.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = m_dx12_->CompileShader(psPath, L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// DepthStencilState
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// depthの機能を無効にする
	depthStencilDesc.DepthEnable = false;

	// PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignatures_[effectName].Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	hr = m_dx12_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipelineStates_[effectName]));
	assert(SUCCEEDED(hr));
}

void PostEffect::CreateVignetteParam()
{
	// VignetteParamのリソース生成
	vignetteParamResource_ = m_dx12_->MakeBufferResource(sizeof(VignetteParam));

	// データの設定
	vignetteParamResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteParam_));

	// データの初期化
	vignetteParam_->power = 0.0f;
	vignetteParam_->range = 20.0f;
}

void PostEffect::CreateVignetteRedBloomParam()
{
	// VignetteRedBloomParamのリソース生成
	vignetteRedBloomParamResource_ = m_dx12_->MakeBufferResource(sizeof(VignetteRedBloomParam));

	// データの設定
	vignetteRedBloomParamResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteRedBloomParam_));

	// データの初期化
	vignetteRedBloomParam_->power = 0.0f;
	vignetteRedBloomParam_->threshold = 1.0f;
	vignetteRedBloomParam_->range = 20.0f;
}

void PostEffect::CreateBloomParam()
{
	// BloomParamのリソース生成
	bloomParamResource_ = m_dx12_->MakeBufferResource(sizeof(BloomParam));

	// データの設定
	bloomParamResource_->Map(0, nullptr, reinterpret_cast<void**>(&bloomParam_));

	// データの初期化
	bloomParam_->intensity = 1.0f;
	bloomParam_->threshold = 0.9f;
	bloomParam_->sigma = 2.0f;
}

void PostEffect::CreateFogParam()
{
	// FogParamのリソース生成
	fogParamResource_ = m_dx12_->MakeBufferResource(sizeof(FogParam));

	// データの設定
	fogParamResource_->Map(0, nullptr, reinterpret_cast<void**>(&fogParam_));

	// データの初期化
	fogParam_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	fogParam_->density = 0.05f;
}

void PostEffect::CreateCameraForGPU()
{
	// CameraForGPUのリソース生成
	cameraForGPUResource_ = m_dx12_->MakeBufferResource(sizeof(CameraForGPU));

	// データの設定
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));

	// データの初期化
	cameraForGPU_->farPlane = Object3dBasic::GetInstance()->GetCamera()->GetFarClip();
	cameraForGPU_->nearPlane = Object3dBasic::GetInstance()->GetCamera()->GetNearClip();
}

void PostEffect::SetParamResource(const std::string& effectName)
{
	if (effectName == "VignetteRed" || effectName == "VigRedGrayScale")
	{
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, vignetteParamResource_->GetGPUVirtualAddress());
	} else if (effectName == "VignetteRedBloom")
	{
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, vignetteRedBloomParamResource_->GetGPUVirtualAddress());
	} else if (effectName == "Bloom")
	{
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, bloomParamResource_->GetGPUVirtualAddress());
	} else if (effectName == "BloomFog")
	{
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, bloomParamResource_->GetGPUVirtualAddress());
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(2, cameraForGPUResource_->GetGPUVirtualAddress());
		m_dx12_->GetCommandList()->SetGraphicsRootConstantBufferView(3, fogParamResource_->GetGPUVirtualAddress());
	} else if (effectName == "GrayScale" || effectName == "NoEffect")
	{
		// グレースケール, ノーエフェクトの場合は何もしない
	}
}
