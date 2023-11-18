#include "main.h"
#include "renderer.h"
#include "Goal.h"
#include "manager.h"
#include "scene.h"
#include "title.h"
#include "player.h"

void GOAL::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
	
	m_Clear = false;
}

void GOAL::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void GOAL::Update()
{
	m_Rotation.y += 50.0f;

	Scene* scene = Manager::GetScene();
	std::vector<PLAYER*> players = scene->GetGameObjects<PLAYER>();
	for (PLAYER* player : players)
	{
		D3DXVECTOR3 playerPos = player->GetPosition();
		D3DXVECTOR3 direction = playerPos - m_Position;
		float length = D3DXVec3Length(&direction);

		if (length < 1.5f)
		{	m_Clear = true;	}
	}
}

void GOAL::Draw()
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


