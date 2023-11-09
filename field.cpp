#include "main.h"
#include "renderer.h"
#include "field.h"

#define	FIELD_SIZE (90.0f)

void FIELD::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position	= D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
	vertex[0].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position	= D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
	vertex[1].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= D3DXVECTOR2(FIELD_SIZE/2.0f, 0.0f);

	vertex[2].Position	= D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE);
	vertex[2].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= D3DXVECTOR2(0.0f, FIELD_SIZE / 2.0f);

	vertex[3].Position	= D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE);
	vertex[3].Normal	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= D3DXVECTOR2(FIELD_SIZE / 2.0f, FIELD_SIZE / 2.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(VERTEX_3D) * 4;
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags	= 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\tile01.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}

void FIELD::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void FIELD::Update()
{

}

void FIELD::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader,  NULL, 0);

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
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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