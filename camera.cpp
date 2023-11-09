#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "EntityPlayer.h"
#include "Weapon.h"

void CAMERA::Init()
{
	m_Position  = D3DXVECTOR3(0.0f, 3.5f, -10.0f);
	m_Target	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_ShakeVe	= D3DXVECTOR3(0.1f, 0.1f, 0.0f);
	m_Velocity  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void CAMERA::Update()
{
	Scene* scene = Manager::GetScene();
	EntityPlayer*	player = scene->GetGameObject<EntityPlayer>();
	WEAPON*			weapon = scene->GetGameObject<WEAPON>();
	//�g�b�v
	//m_Target = player->GetPosition();
	//m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	//m_Position += D3DXVECTOR3(cosf(m_Rotation.y), 0.0f, sinf(m_Rotation.y)) * 5.0f;

	//�T�[�h�p�[�\��
	//m_Target = player->GetPosition()
	//	+ player->GetRight() * 0.5f
	//	+ D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Position = m_Target - player->GetForward() * 20.0f 
	//	+ D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (weapon->GetAiming())
	{
		if (m_Position.z <= player->GetPosition().z-24.0f)
		{
			m_Position.z += m_Velocity.z;
		}
	}
	else if (m_Position.z >= player->GetPosition().z - 25.0f)
	{
		m_Position.z -= m_Velocity.z;
	}

	float y = player->GetPosition().y + 1.0f;

	if (player->GetSneak())
	{
		y += PLAYER_SIZEY / 4.0f;
	}

	m_Position = D3DXVECTOR3(player->GetPosition().x, y, m_Position.z);
	m_Target = D3DXVECTOR3(player->GetPosition().x, m_Position.y, 0.0f);

	////�t�@�[�X�g�p�[�\��
	//m_Position = player->GetPosition()
	//	+ D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	////m_Position = player->GetPosition()
	////	+ player->GetRight()
	////	+ CameraShake();
	//m_Target = m_Position - player->GetForward() * -1;

}

void CAMERA::Draw()
{
	//�r���[�}�g���b�N�X�ݒ�
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

D3DXVECTOR3 CAMERA::CameraShake()
{
	
	m_ShakeVe.y * 1.01f;

	if (m_ShakeVe.y > 0.3f)
	{
		m_ShakeVe.y = -0.1f;
	}
	if (m_ShakeVe.y < -0.3f)
	{
		m_ShakeVe.y = 0.1f;
	}

	D3DXVECTOR3 newPos = D3DXVECTOR3(0.0f,m_ShakeVe.y+1.0f,0.0f);

	return newPos;
}



