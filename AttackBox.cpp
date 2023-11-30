#include "main.h"
#include "renderer.h"
#include "AttackBox.h"

#define DEFAULT_SIZE (5.0f)

void AttackBox::Init()
{
	m_Active	= false;
	m_Count		= 0;
	m_Rotation	= { 1.0f,1.0f,1.0f };
	m_Position	= { 1.0f,1.0f,1.0f };
	m_Scale		= { 1.0f,1.0f,1.0f };
	m_Color		= { 1.0f,1.0f,1.0f,1.0f };

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-DEFAULT_SIZE, 0.0f, DEFAULT_SIZE);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(DEFAULT_SIZE, 0.0f, DEFAULT_SIZE);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-DEFAULT_SIZE, 0.0f, -DEFAULT_SIZE);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(DEFAULT_SIZE, 0.0f, -DEFAULT_SIZE);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

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
		"asset\\texture\\EffectExplosion.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_TexX = 4;
	m_TexY = 4;
	m_TexCount = 0;
}

void AttackBox::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void AttackBox::Update()
{
	if (m_Count > 0)
	{
		m_Count--;
		m_TexCount++;
	}
	else
	{
		m_Active = false;
	}
	
}

void AttackBox::Draw()
{
	if (m_Active)
	{
		float x = m_TexCount % m_TexX * (1.0f / m_TexX);
		float y = m_TexCount / m_TexY * (1.0f / m_TexY);

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position	= D3DXVECTOR3(-DEFAULT_SIZE, 0.0f, DEFAULT_SIZE);
		vertex[0].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord  = D3DXVECTOR2(x, y);

		vertex[1].Position	= D3DXVECTOR3(DEFAULT_SIZE, 0.0f, DEFAULT_SIZE);
		vertex[1].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord	= D3DXVECTOR2(x + 1.0f / m_TexX, y);

		vertex[2].Position	= D3DXVECTOR3(-DEFAULT_SIZE, 0.0f, -DEFAULT_SIZE);
		vertex[2].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord	= D3DXVECTOR2(x, y + +1.0f / m_TexY);

		vertex[3].Position	= D3DXVECTOR3(DEFAULT_SIZE, 0.0f, -DEFAULT_SIZE);
		vertex[3].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord	= D3DXVECTOR2(x + 1.0f / m_TexX, y + 1.0f / m_TexY);


		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダ設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->
			IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = m_Color;
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		//テクスチャ設定
		Renderer::GetDeviceContext()
			->PSSetShaderResources(0, 1, &m_Texture);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()
			->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void AttackBox::SetActive(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot, int cnt)
{
	if (!m_Active)
	{
		m_Position = pos;
		m_Scale = scale;
		m_Rotation = rot;
		m_Count = cnt;
		m_Active = true;
		m_TexCount = 0;
	}
}

void AttackBox::SetColor(D3DXCOLOR col)
{
	m_Color = col;
}

bool AttackBox::GetActive()
{
	return m_Active;
}