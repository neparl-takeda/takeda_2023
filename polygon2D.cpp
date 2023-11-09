
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sprite.h"

void POLYGON2D::Init()
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	AddComponent<SPRITE>()
		->Init(000.0f, 000.0f, 200.0f, 200.0f, "asset\\texture\\Back.JPG");
	AddComponent<SPRITE>()
		->Init(200.0f, 000.0f, 200.0f, 200.0f, "asset\\texture\\tile01.png");
}

void POLYGON2D::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void POLYGON2D::Update()
{
	GameObject::Update();
}

void POLYGON2D::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//基底クラスのメソッド呼び出し
	GameObject::Draw();
}