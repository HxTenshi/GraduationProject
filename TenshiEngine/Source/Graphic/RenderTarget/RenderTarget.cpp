#pragma once

#include "RenderTarget.h"
#include "Game/RenderingSystem.h"


RenderTarget::RenderTarget()
		: mpRenderTargetView(NULL)
		, mpTexture2D(NULL)
		, mpDepthStencilView(NULL)
		, mpDepthTexture2D(NULL)
		, m_Width(0)
		, m_Height(0)
		, m_Format(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)
	{
	}
RenderTarget::~RenderTarget()
	{
	}
	HRESULT RenderTarget::Create(UINT Width, UINT Height, DXGI_FORMAT format)
	{
		m_Width = Width;
		m_Height = Height;
		m_Format = format;
		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = m_Width;
		tex_desc.Height = m_Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		//tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex_desc.Format = m_Format;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		//�����_�[�^�[�Q�b�g�Ƃ��Ďg�p���V�F�[�_���\�[�X�Ƃ��ė��p
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;

		//�f�v�X�e�N�X�`���̏ꍇ
		//tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		if (!_CreateTexture2D(&tex_desc))return E_FAIL;

		if (!_CreateRenderTargetView())return E_FAIL;

		if (!_CreateTexture())return E_FAIL;

		auto render = RenderingEngine::GetEngine(ContextType::Create);
		ClearView(render->m_Context);

		return S_OK;
	}

	HRESULT RenderTarget::CreateRTandDepth(UINT Width, UINT Height, DXGI_FORMAT format)
	{

		Create(Width, Height, format);
		CreateDepth(Width, Height);

		return S_OK;
	}
	HRESULT RenderTarget::CreateCUBE(UINT Width, UINT Height, DXGI_FORMAT format)
	{
		m_Width = Width;
		m_Height = Height;
		m_Format = format;
		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = m_Width;
		tex_desc.Height = m_Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 6;
		//tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex_desc.Format = m_Format;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		//�����_�[�^�[�Q�b�g�Ƃ��Ďg�p���V�F�[�_���\�[�X�Ƃ��ė��p
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


		if (!_CreateTexture2D(&tex_desc))return E_FAIL;

		if (!_CreateRenderTargetView())return E_FAIL;

		if (!_CreateTexture())return E_FAIL;

		auto render = RenderingEngine::GetEngine(ContextType::Create);
		ClearView(render->m_Context);

		return S_OK;
	}

	HRESULT RenderTarget::CreateDepth(UINT Width, UINT Height)
	{
		m_Width = Width;
		m_Height = Height;
		//�f�v�X�o�b�t�@
		//��DXGI_FORMAT_D24_UNORM_S8_UINT
		//���x�ɖ��Ȃ���΂�����̗p
		//��DXGI_FORMAT_D32_FLOAT
		//�f�v�X�l�̐��x���K�v�ł����
		//��DXGI_FORMAT_R24G8_TYPELESS
		//�f�v�X�l�A�N�Z�X�̂��߂̓���t�H�[�}�b�g�A����Ńe�N�X�`���쐬
		//CreateDepthStencilView����D24_UNORM_S8_UINT�ɕϊ�
		//��DXGI_FORMAT_R24_UNORM_X8_TYPELESS
		//��DXGI_FORMAT_X24_TYPELESS_G8_UINT
		//�V�F�[�_�Ńf�v�X�X�e���V���l���擾���邽�߂̃t�H�[�}�b�g
		//CreateShaderResourceView�Ŏw��

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = m_Width;
		tex_desc.Height = m_Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		//tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;


		if (!_CreateDepthTexture2D(&tex_desc))return E_FAIL;

		//Width�AHeight�͑S�����_�[�^�[�Q�b�g�ƃf�v�X�o�b�t�@���ׂē����ɂ���
		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		// �f�v�X�o�b�t�@
		if (!_CreateDepthStencilView(&descDSV))return E_FAIL;

		auto render = RenderingEngine::GetEngine(ContextType::Create);
		ClearDepth(render->m_Context);

		//ID3D11ShaderResourceView* pShaderResourceView;
		//hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		//if (FAILED(hr))
		//	return hr;
		//
		//mTexture.Create(shared_ptr<ID3D11ShaderResourceView>(pShaderResourceView));


		return S_OK;
	}

	HRESULT RenderTarget::CreateBackBuffer(UINT Width, UINT Height)
	{
		HRESULT hr = S_OK;

		m_Width = Width;
		m_Height = Height;

		// Create a render target view
		hr = Device::mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mpTexture2D);
		if (FAILED(hr))
			return hr;

		//�����_�[�^�[�Q�b�g�̍쐬
		if (!_CreateRenderTargetView())return E_FAIL;

		return S_OK;
	}

	bool RenderTarget::Resize(UINT Width, UINT Height) {
		if (mpTexture2D) {
			mpTexture2D->Release();
			mpTexture2D = NULL;


			m_Width = Width;
			m_Height = Height;
			// Create depth stencil texture
			D3D11_TEXTURE2D_DESC tex_desc;
			ZeroMemory(&tex_desc, sizeof(tex_desc));
			tex_desc.Width = m_Width;
			tex_desc.Height = m_Height;
			tex_desc.MipLevels = 1;
			tex_desc.ArraySize = 1;
			tex_desc.Format = m_Format;
			tex_desc.SampleDesc.Count = 1;
			tex_desc.SampleDesc.Quality = 0;
			tex_desc.Usage = D3D11_USAGE_DEFAULT;
			tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			tex_desc.CPUAccessFlags = 0;
			tex_desc.MiscFlags = 0;

			if (!_CreateTexture2D(&tex_desc))return false;
		}
		auto render = RenderingEngine::GetEngine(ContextType::MainDeferrd);
		if (mpRenderTargetView) {
			mpRenderTargetView->Release();
			mpRenderTargetView = NULL;

			if (!_CreateRenderTargetView())return false;
			ClearView(render->m_Context);
		}


		if (mpDepthTexture2D) {
			mpDepthTexture2D->Release();
			mpDepthTexture2D = NULL;


			m_Width = Width;
			m_Height = Height;
			// Create depth stencil texture
			D3D11_TEXTURE2D_DESC tex_desc;
			ZeroMemory(&tex_desc, sizeof(tex_desc));
			tex_desc.Width = m_Width;
			tex_desc.Height = m_Height;
			tex_desc.MipLevels = 1;
			tex_desc.ArraySize = 1;
			tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
			tex_desc.SampleDesc.Count = 1;
			tex_desc.SampleDesc.Quality = 0;
			tex_desc.Usage = D3D11_USAGE_DEFAULT;
			tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			tex_desc.CPUAccessFlags = 0;
			tex_desc.MiscFlags = 0;

			if (!_CreateDepthTexture2D(&tex_desc))return false;
		}
		if (mpDepthStencilView) {
			mpDepthStencilView->Release();
			mpDepthStencilView = NULL;

			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D32_FLOAT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			if (!_CreateDepthStencilView(&descDSV))return false;
			ClearDepth(render->m_Context);

		}
		if (!_CreateTexture())return false;


		return true;
	}

	void RenderTarget::ClearView(ID3D11DeviceContext* context) const {
		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0,0,0, 1.0f };
		if (mpRenderTargetView)context->ClearRenderTargetView(mpRenderTargetView, ClearColor);
	}
	void RenderTarget::ClearDepth(ID3D11DeviceContext* context) const {
		if (mpDepthStencilView)context->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	Texture& RenderTarget::GetTexture() {
		return mTexture;
	}

	void RenderTarget::PSSetShaderResource(ID3D11DeviceContext* context, UINT Slot) const {
		mTexture.PSSetShaderResources(context, Slot);
	}

	void RenderTarget::SetRendererTarget(ID3D11DeviceContext* context) const {
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��

		if (mpRenderTargetView) {
			_SetViewport(context, *this);
			context->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);
		}
	}
	//static
	void RenderTarget::SetRendererTarget(ID3D11DeviceContext* context, UINT num, const RenderTarget* render, const RenderTarget* depth) {

		_ASSERT(num<9);
		_SetViewport(context, render[0]);

		ID3D11RenderTargetView* r[8];
		for (UINT i = 0; i < num; i++) {
			r[i] = render[i].mpRenderTargetView;
		}
		ID3D11DepthStencilView* d = depth ? depth->mpDepthStencilView : NULL;
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		context->OMSetRenderTargets(num, r, d);
	}
	//static
	void RenderTarget::NullSetRendererTarget(ID3D11DeviceContext* context) {

		ID3D11RenderTargetView* r[8];
		for (UINT i = 0; i < 8; i++) {
			r[i] = NULL;
		}
		//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̊֘A�t��
		context->OMSetRenderTargets(8, r, NULL);
	}

	void RenderTarget::Release()
	{
		if (mpTexture2D) {
			mpTexture2D->Release();
			mpTexture2D = NULL;
		}
		if (mpDepthTexture2D) {
			mpDepthTexture2D->Release();
			mpDepthTexture2D = NULL;
		}
		//ID3D11RenderTargetView* rt[8];
		//ID3D11DepthStencilView* ds;
		//Device::mpImmediateContext->OMGetRenderTargets(8, rt, &ds);

		if (mpDepthStencilView) {
			//if (ds == mpDepthStencilView){
			//	NullSetRendererTarget();
			//}
			mpDepthStencilView->Release();
			mpDepthStencilView = NULL;
		}
		if (mpRenderTargetView) {
			//for (int i = 0; i < 8; i++){
			//	if (rt[i] == mpRenderTargetView){
			//		NullSetRendererTarget();
			//	}
			//}
			mpRenderTargetView->Release();
			mpRenderTargetView = NULL;
		}

	}

	//static
	void RenderTarget::_SetViewport(ID3D11DeviceContext* context, const RenderTarget& rt) {
		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)rt.m_Width;
		vp.Height = (FLOAT)rt.m_Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);
	}

	bool RenderTarget::_CreateTexture2D(const D3D11_TEXTURE2D_DESC* tex_desc) {
		auto hr = Device::mpd3dDevice->CreateTexture2D(tex_desc, nullptr, &mpTexture2D);
		if (FAILED(hr)) {
			_SYSTEM_LOG_ERROR("Texture2D�̍쐬");
			return false;
		}
		return true;
	}
	bool RenderTarget::_CreateDepthTexture2D(const D3D11_TEXTURE2D_DESC* tex_desc) {
		auto hr = Device::mpd3dDevice->CreateTexture2D(tex_desc, nullptr, &mpDepthTexture2D);
		if (FAILED(hr)) {
			_SYSTEM_LOG_ERROR("DepthTexture2D�̍쐬");
			return false;
		}
		return true;
	}

	bool RenderTarget::_CreateRenderTargetView() {
		// RenderTargetView�쐬�@MRT�ɕK�v�Ȍ�
		auto hr = Device::mpd3dDevice->CreateRenderTargetView(mpTexture2D, nullptr, &mpRenderTargetView);
		if (FAILED(hr)) {
			_SYSTEM_LOG_ERROR("RenderTargetView�̍쐬");
			return false;
		}
		return true;
	}
	bool RenderTarget::_CreateTexture() {
		ID3D11ShaderResourceView* pShaderResourceView;
		auto hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		if (FAILED(hr)) {
			_SYSTEM_LOG_ERROR("ShaderResourceView�̍쐬");
			return false;
		}

		hr = mTexture.Create(pShaderResourceView);
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}

	bool RenderTarget::_CreateDepthStencilView(const D3D11_DEPTH_STENCIL_VIEW_DESC* desc) {
		auto hr = Device::mpd3dDevice->CreateDepthStencilView(mpDepthTexture2D, desc, &mpDepthStencilView);
		if (FAILED(hr)) {
			_SYSTEM_LOG_ERROR("DepthStencilView�̍쐬");
			return false;
		}
		return true;
	}

	UAVRenderTarget::UAVRenderTarget()
		: m_UAV(NULL)
		, mpTexture2D(NULL)
	{
	}
	UAVRenderTarget::~UAVRenderTarget()
	{
	}
	HRESULT UAVRenderTarget::Create(UINT Width, UINT Height, DXGI_FORMAT format)
	{
		HRESULT hr = S_OK;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(tex_desc));
		tex_desc.Width = Width;
		tex_desc.Height = Height;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		//tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex_desc.Format = format;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		//�����_�[�^�[�Q�b�g�Ƃ��Ďg�p���V�F�[�_���\�[�X�Ƃ��ė��p
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;


		//�f�v�X�e�N�X�`���̏ꍇ
		//tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		//���̃����o�͏ȗ�
		hr = Device::mpd3dDevice->CreateTexture2D(&tex_desc, nullptr, &mpTexture2D);
		if (FAILED(hr))
			return hr;

		ID3D11ShaderResourceView* pShaderResourceView;
		hr = Device::mpd3dDevice->CreateShaderResourceView(mpTexture2D, nullptr, &pShaderResourceView);
		if (FAILED(hr))
			return hr;

		hr = mTexture.Create(pShaderResourceView);
		if (FAILED(hr))
			return hr;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = 0;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		Device::mpd3dDevice->CreateUnorderedAccessView(mpTexture2D, &uavDesc, &m_UAV);


		auto render = RenderingEngine::GetEngine(ContextType::Create);
		ClearView(render->m_Context);

		return S_OK;
	}
	void UAVRenderTarget::ClearView(ID3D11DeviceContext* context) const {
		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0, 0, 0, 1.0f };
		if (m_UAV)context->ClearUnorderedAccessViewFloat(m_UAV, ClearColor);
	}

	void UAVRenderTarget::SetUnorderedAccessView(ID3D11DeviceContext* context) const {
		if (m_UAV)
			context->CSSetUnorderedAccessViews(0, 1, &m_UAV, (UINT*)&m_UAV);
	}

	void UAVRenderTarget::Release()
	{
		if (mpTexture2D) {
			mpTexture2D->Release();
			mpTexture2D = NULL;
		}
		if (m_UAV) {
			m_UAV->Release();
			m_UAV = NULL;
		}

	}

	Texture& UAVRenderTarget::GetTexture() {
		return mTexture;
	}

	void UAVRenderTarget::PSSetShaderResource(ID3D11DeviceContext* context, UINT Slot) const {
		mTexture.PSSetShaderResources(context, Slot);
	}