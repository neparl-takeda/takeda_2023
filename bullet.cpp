#include "main.h"
#include "renderer.h"
#include "bullet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "explosion.h"
#include "EffectKira.h"
#include "enemy.h"
#include "Score.h"
#include "audio.h"

//SCORE score;

void BULLET::Init()
{
	Scene* scene = Manager::GetScene();
	PLAYER* player = scene->GetGameObject<PLAYER>();

	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Velocity  = D3DXVECTOR3(0.0f, 0.0f, 0.4f);
	m_Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Rotation  = player->GetRotation();
	m_Scale		= D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_DamageSE = AddComponent<Audio>();
	m_DamageSE->Load("asset\\sound\\SE_bullet.wav");
}

void BULLET::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void BULLET::Update()
{
	//m_Position += m_Velocity;
	m_Position += GetForward() * m_Velocity.z;

	if ((m_Position.z > 20.0f)||(m_Position.z<-20.0f)|| (m_Position.x > 20.0f) || (m_Position.x < -20.0f))
	{
		SetDestroy();
	}

	Scene* scene = Manager::GetScene();
	std::vector<ENEMY*> enemys = scene->GetGameObjects<ENEMY>();
	for(ENEMY* enemy:enemys)
	{
		D3DXVECTOR3 enemyPos  = enemy->GetPosition();
		D3DXVECTOR3 direction = enemyPos-m_Position;
		float length = D3DXVec3Length(&direction);
		
		if (length<1.0f)
		{
			scene->AddGameObject<EXPLOSION>(1)->SetPosition(enemyPos+D3DXVECTOR3(0.0f,1.0f,0.0f));

			SetDestroy();
			enemy->SetDestroy();

			SCORE* score = scene->GetGameObject<SCORE>();
			score->AddScore(5);

			m_DamageSE->Play();
			return;
		}

	}
}

void BULLET::Draw()
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
