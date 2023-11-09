#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "TerrainBoxMove.h"

void TerrainBoxMove::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\box.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void TerrainBoxMove::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void TerrainBoxMove::Update()
{
	m_Position.y += m_Count * m_Spd;

	if (m_Count > m_Max)
	{
		m_Add = -1;
	}
	if (m_Count < -m_Max)
	{
		m_Add = 1;
	}

	m_Count += m_Add;
}

void TerrainBoxMove::Draw()
{
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

	m_Model->Draw();
}

void TerrainBoxMove::SetParam(D3DXVECTOR3 pos, D3DXVECTOR3 scl, int max, float spd)
{
	m_Position		= pos;
	m_Scale			= scl;
	
}
