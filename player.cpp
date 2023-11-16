#include "main.h"
#include "renderer.h"
#include "player.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"

#define DIRECTION_DISTANCE	(3.15f/2.0f)
#define DIRECTION_W		( DIRECTION_DISTANCE * 0.5f)
#define DIRECTION_WD	( DIRECTION_DISTANCE * 1.0f)
#define DIRECTION_D		( DIRECTION_DISTANCE * 1.5f)
#define DIRECTION_DS	( DIRECTION_DISTANCE * 2.0f)
#define DIRECTION_S		( DIRECTION_DISTANCE * 2.5f)
#define DIRECTION_SA	( DIRECTION_DISTANCE * 3.0f)
#define DIRECTION_A		( DIRECTION_DISTANCE * 3.5f)
#define DIRECTION_AW	( DIRECTION_DISTANCE * 4.0f)




bool pushBulletKey = false;

void PLAYER::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\player.obj");

	//m_Position	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Velocity		= { 0.0f,0.0f,0.0f };
	m_RotationAtk	= { 0.0f,0.0f,0.0f };

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_ShotSE = AddComponent<Audio>();
	m_ShotSE->Load("asset\\sound\\SE_bullet.wav");
}

void PLAYER::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void PLAYER::Update()
{
	D3DXVECTOR3 oldPosition = m_Position;
	m_Velocity = { 0.0f,0.0f,0.0f };

	Scene* scene = Manager::GetScene();
	////top view
	//if (Input::GetKeyPress('A'))
	//{	m_Position.x -= 0.1f;
	//	m_Rotation.y = -D3DX_PI * 0.5f;	}
	//if (Input::GetKeyPress('D'))
	//{	m_Position.x += 0.1f;
	//	m_Rotation.y = D3DX_PI * 0.5f;	}
	//if (Input::GetKeyPress('W'))
	//{	m_Position.z += 0.1f;	}
	//if (Input::GetKeyPress('S'))
	//{	m_Position.z -= 0.1f;	}

	//if (Input::GetKeyPress('Q'))
	//{	m_Rotation.y -= 0.1f;	}
	//if (Input::GetKeyPress('E'))
	//{	m_Rotation.y += 0.1f;	}

	//tps fps

	PlayerRotMath();

	//現在座標に移動ベクトルを足す
	if (Input::GetKeyPress('W') || Input::GetKeyPress('A') || Input::GetKeyPress('S') || Input::GetKeyPress('D'))
	{	m_Position += GetForward() * 0.1f;	}

	m_Position += m_Velocity;

	if (Input::GetKeyPress('G'))
	{
		if (!pushBulletKey)
		{
			(m_Position + D3DXVECTOR3(0.0f, 1.0f, 0.0f)
				, m_Rotation, D3DXVECTOR3(0.5f, 0.5f, 0.5f));
			m_ShotSE->Play();
		}
		pushBulletKey = true;	
	}

	if (!Input::GetKeyPress('G'))
	{	pushBulletKey = false;	}

	//衝突判定
	float groundHeight = 0.0f;


		//cylinder
		std::vector<CYLINDER*> cylinders = scene->GetGameObjects<CYLINDER>();
		for (CYLINDER* cylinder : cylinders)
		{
			D3DXVECTOR3 position = cylinder->GetPosition();
			D3DXVECTOR3 scale    = cylinder->GetScale();
			
			//groundHeight = position.y + scale.y / 2.0f;

			D3DXVECTOR3 direction = m_Position - position;
			direction.y = 0.0f;
			float length = D3DXVec3Length(&direction);
			if (length < scale.x)
			{
				if (m_Position.y < position.y + scale.y - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{	groundHeight = position.y + scale.y;	}
			}
		}

		//box
		std::vector<BOX*> boxes = scene->GetGameObjects<BOX>();
		for (BOX* box : boxes)
		{
			D3DXVECTOR3 position = box->GetPosition();
			D3DXVECTOR3 scale	 = box->GetScale();

			if (position.x - scale.x - 0.5f < m_Position.x &&
				m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z &&
				m_Position.z < position.z + scale.z + 0.5f
				)
			{
				if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{	groundHeight = position.y + scale.y * 2.0f;	}

			}
		
		}

	//ジャンプ
	if (m_Position.y < groundHeight + 0.1f && m_Velocity.y < 0.1f)
	{
		if (Input::GetKeyTrigger(VK_SPACE))
		{	m_Velocity.y = 0.35f;	}
	}
	m_Position += m_Velocity;	//
	m_Velocity.y -= 0.015f;		//重力

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
}

void PLAYER::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

bool PLAYER::GetMoving()
{
	bool moving = false;
	if ((Input::GetKeyPress('A')) ||
		(Input::GetKeyPress('S')) ||
		(Input::GetKeyPress('W')) ||
		(Input::GetKeyPress('D')))
	{
		moving = true;
	}
	return moving;
}

void PLAYER::PlayerRotMath()
{
	if (Input::GetKeyPress('A'))
	{	m_Rotation.y = DIRECTION_A;	}

	if (Input::GetKeyPress('D'))
	{	m_Rotation.y = DIRECTION_D;	}

	if (Input::GetKeyPress('W'))
	{	m_Rotation.y = DIRECTION_W;	}

	if (Input::GetKeyPress('S'))
	{	m_Rotation.y = DIRECTION_S;	}

	if (Input::GetKeyPress('A') && Input::GetKeyPress('W'))
	{	m_Rotation.y = DIRECTION_AW;	}

	if (Input::GetKeyPress('A') && Input::GetKeyPress('S'))
	{	m_Rotation.y = DIRECTION_SA;	}

	if (Input::GetKeyPress('D') && Input::GetKeyPress('W'))
	{	m_Rotation.y = DIRECTION_WD;	}

	if (Input::GetKeyPress('D') && Input::GetKeyPress('S'))
	{	m_Rotation.y = DIRECTION_DS;	}

}
