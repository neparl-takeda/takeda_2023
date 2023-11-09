#include "main.h"
#include "renderer.h"
#include "bulletVortex.h"
#include "bulletSplit.h"
#include "bullet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "explosion.h"
#include "EffectKira.h"
#include "enemy.h"


void BULLETVORTEX::Init()
{
	Scene* scene = Manager::GetScene();
	PLAYER* player = scene->GetGameObject<PLAYER>();

	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");
	m_Count = 0;

	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.4f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = player->GetRotation();
	m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");


}

void BULLETVORTEX::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void BULLETVORTEX::Update()
{
	//m_Position += m_Velocity;
	m_Position += GetForward() * m_Velocity.z;

	if ((m_Position.z > 20.0f) || (m_Position.z < -20.0f) || (m_Position.x > 20.0f) || (m_Position.x < -20.0f))
	{
		SetDestroy();
	}

	Scene* scene = Manager::GetScene();
	std::vector<ENEMY*> enemys = scene->GetGameObjects<ENEMY>();
	for (ENEMY* enemy : enemys)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();
		D3DXVECTOR3 direction = enemyPos - m_Position;
		float length = D3DXVec3Length(&direction);

		if (length < 1.0f)
		{
			scene->AddGameObject<EXPLOSION>(1)->SetPosition(enemyPos);

			SetDestroy();
			enemy->SetDestroy();
			return;
		}

	}
	m_Count++;
	if ((m_Count >= 25)&&(m_Count%3 == 0))
	{
			scene->AddGameObject<BULLET>(1)
				->SetTransform(m_Position,
					m_Rotation +
					D3DXVECTOR3(0.0f, -5.0f + (m_Count/3) * 1.5f, 0.0f),
					D3DXVECTOR3(0.2f, 0.2f, 0.2f));
	}
	if (m_Count >= 120)
	{
		SetDestroy();
	}
	m_Scale -= D3DXVECTOR3(0.0025f,0.0025f,0.0025f);
}

void BULLETVORTEX::Draw()
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
