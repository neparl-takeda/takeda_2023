#include "main.h"
#include "renderer.h"
#include "Ebullet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "explosion.h"
#include "EffectKira.h"
#include "enemy.h"
#include "Score.h"
#include "audio.h"
#include "life.h"

//SCORE score;

void E_BULLET::Init()
{
	Scene* scene = Manager::GetScene();
	ENEMY* enemy = scene->GetGameObject<ENEMY>();

	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.4f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Rotation  = player->GetRotation();
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_DamageSE = AddComponent<Audio>();
	m_DamageSE->Load("asset\\sound\\SE_bullet.wav");
}

void E_BULLET::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void E_BULLET::Update()
{
	//m_Position += m_Velocity;
	m_Position += GetForward() * m_Velocity.z;

	if ((m_Position.z > 100.0f) || (m_Position.z < -100.0f) || (m_Position.x > 100.0f) || (m_Position.x < -100.0f))
	{
		SetDestroy();
	}

	Scene* scene = Manager::GetScene();
	std::vector<PLAYER*> players = scene->GetGameObjects<PLAYER>();
	for (PLAYER* player : players)
	{
		D3DXVECTOR3 playerPos = player->GetPosition();
		D3DXVECTOR3 direction = playerPos - m_Position;
		float length = D3DXVec3Length(&direction);

		if (length < 1.0f)
		{
			scene->AddGameObject<EXPLOSION>(1)->SetPosition(playerPos + D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			SetDestroy();
			//enemy->SetDestroy();
			LIFE* life = scene->GetGameObject<LIFE>();
			life->AddLife(-1);

			SCORE* score = scene->GetGameObject<SCORE>();
			score->AddScore(5);

			m_DamageSE->Play();
			return;
		}

	}
}

void E_BULLET::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
