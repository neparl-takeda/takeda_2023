
#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sprite.h"

//#include "input.h"

void Fade::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<SPRITE>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\ScreenFade.png");
}

void Fade::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Fade::Update()
{
	GameObject::Update();

	if (m_In)
	{
		m_Alpha -= 0.02f;
		if (m_Alpha < 0.0f)
		{
			m_FinishedIn = true;
			m_Alpha = 0.0f;
		}
	}
	else
	{
		m_Alpha += 0.02f;
		if (m_Alpha > 1.0f)
		{
			m_FinishedOut = true;
			m_Alpha = 1.0f;
		}
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}

void Fade::Draw()
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