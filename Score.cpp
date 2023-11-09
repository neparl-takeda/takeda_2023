
#include "main.h"
#include "renderer.h"
#include "Score.h"
#include "sprite.h"

#define SCORE_SIZE (10.0f)
bool minus = false;

void SCORE::Init()
{	
	VERTEX_3D vertex[4];

	vertex[0].Position	= D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	vertex[0].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position	= D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	vertex[1].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= D3DXVECTOR2(10.0f, 0.0f);

	vertex[2].Position	= D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	vertex[2].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= D3DXVECTOR2(0.0f, 10.0f);

	vertex[3].Position	= D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	vertex[3].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= D3DXVECTOR2(10.0f, 10.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\number.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Score = 0;

	for (int i = 0; i < 3; i++)
	{
		m_NumCol[i] = D3DXVECTOR4(0.4f, 0.4f, 0.4f, 0.0f);
	}
	m_NumCol[0] = D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f);
}

void SCORE::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void SCORE::Update()
{

	GameObject::Update();

	if (m_Score < 0)
	{
		//m_Score = 0;
		//m_Score *= -1;
		minus = true;
	}
	//m_NumCol[0] = m_ColA[0];
	if (m_Score > 0)
	{
		m_NumCol[0] = m_ColB[0];
		minus = false;
	}
	else
	{
		m_NumCol[0] = m_ColA[0];
	}

	if (m_Score >= 10)
	{
		m_NumCol[1] = m_ColB[1];
	}
	else
	{
		m_NumCol[1] = m_ColA[1];
		m_NumCol[1].w = 0.0f;
	}

	if (m_Score >= 100)
	{
		m_NumCol[2] = m_ColB[2];
	}
	else
	{
		m_NumCol[2] = m_ColA[2];
		m_NumCol[2].w = 0.0f;
	}

	if (m_Score >= 1000)
	{
		m_NumCol[3] = m_ColB[3];
	}
	else
	{
		m_NumCol[3] = m_ColA[3];
		m_NumCol[3].w = 0.0f;
	}
}

void SCORE::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->
		IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()
		->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()
		->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	int count = m_Score;
	if (count < 0)
	{
		count *= -1;
	}
	
	for (int i = 0; i < 4; i++)
	{
		float vx = m_Position.x - i * m_Scale.x;
		float vy = m_Position.y;
		float height = m_Scale.x;
		float width  = m_Scale.y;

		int number = count % 10;
		count /= 10;

		float x = number % 5 * (1.0f / 5);
		float y = number / 5 * (1.0f / 5);

		//float psX = i * 70.0f;

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = m_NumCol[i];
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		vertex[1].Position = D3DXVECTOR3(vx+width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = m_NumCol[i];
		vertex[1].TexCoord = D3DXVECTOR2(x + 0.2f, y);

		vertex[2].Position = D3DXVECTOR3(vx, vy+height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = m_NumCol[i];
		vertex[2].TexCoord = D3DXVECTOR2(x, y + 0.2f);

		vertex[3].Position = D3DXVECTOR3(vx+width, vy+height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = m_NumCol[i];
		vertex[3].TexCoord = D3DXVECTOR2(x + 0.2f, y + 0.2f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void SCORE::SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR4 colA, D3DXVECTOR4 colB,SCORE_TYPE type)
{
	m_Position	= pos;
	m_Scale		= size;
	m_Type		= type;

	for (int i = 0; i < 4; i++)
	{
		m_ColA[i] = colA;
		m_ColB[i] = colB;
		m_NumCol[i] = colA;
	}
	m_NumCol[0].w = 1.0f;
}
