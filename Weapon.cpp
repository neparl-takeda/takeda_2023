#include "main.h"
#include "renderer.h"
#include "Weapon.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "EntityPBullet.h"
#include "EntityPlayer.h"

#include "audio.h"

#define KANDO			(0.03f)
#define LASER_SIZEX		(4.0f)
#define LASER_SIZEY		(5.0f)
#define GUN_ANG			(0.48f)

//#include "ScoreDbg.h"
#include "Score.h"

void WEAPON::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(10.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 10.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(10.0f, 10.0f);

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
		"asset\\texture\\laser.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Moving = false;
	m_Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	Scene* scene = Manager::GetScene();
	
	m_ScoreAmmo		= scene->AddGameObject<SCORE>(3);
	m_ScoreAmmo		->SetScore(D3DXVECTOR3(1200.0f, 200.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), TYPE_AMMO);

	m_ScoreReCnt	= scene->AddGameObject<SCORE>(3);
	m_ScoreReCnt	->SetScore(D3DXVECTOR3(1200.0f, 550.0f, 0.0f), D3DXVECTOR3(25.0f, 25.0f, 0.0f), D3DXVECTOR4(0.4f, 0.0f, 0.0f, 1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), TYPE_RELOAD);

	m_ScoreAngle	= scene->AddGameObject<SCORE>(3);
	m_ScoreAngle	->SetScore(D3DXVECTOR3(1200.0f, 600.0f, 0.0f), D3DXVECTOR3(25.0f, 25.0f, 0.0f), D3DXVECTOR4(0.4f, 0.0f, 0.0f, 1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), TYPE_ANGLE_D);

	m_SEAim  = AddComponent<Audio>();
	m_SEAim	 ->Load("asset\\sound\\\SE_Aim.wav");
	m_SEShot = AddComponent<Audio>();
	m_SEShot ->Load("asset\\sound\\\SE_Shot.wav");

	//SE->Play(true);

	//デバッグ用
	m_Type = TYPE_SHURI;

	switch (m_Type)
	{
	case TYPE_SMG:
		m_Rate = 3;
		m_Maga = 20;
		m_ReTime = 80;
		m_Damage = 0;
		break;
	case TYPE_LMG:
		m_Rate = 10;
		m_Maga = 60;
		m_ReTime = 220;
		m_Damage = 0;
		break;
	case TYPE_AR:
		m_Rate = 6;
		m_Maga = 25;
		m_ReTime = 100;
		m_Damage = 0;
		break;
	case TYPE_DMR:
		m_Rate = 3;
		m_Maga = 21;
		m_ReTime = 110;
		m_Damage = 0;
		break;
	case TYPE_SG:
		m_Rate = 20;
		m_Maga = 8;
		m_ReTime = 100;
		m_Damage = 0;
		break;
	case TYPE_SHURI:
		m_Rate = 12;
		m_Maga = 8;
		m_ReTime = 100;
		m_Damage = 0;
		break;
	default:
		m_Rate	= 10;
		m_Maga	= 8;
		m_ReTime	= 20;
		m_Damage	= 10;
		break;
	}

	m_Ammo = m_Maga;
	m_Ang = 0;
	m_First = false;
}

void WEAPON::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void WEAPON::Update()
{
	bool ShotFlag = false;

	Scene* scene = Manager::GetScene();
	EntityPlayer* player = scene->GetGameObject<EntityPlayer>();
	m_Position = player->GetPosition();

	//タマ
	if ((Input::GetKeyTrigger(VK_RBUTTON)) && (!m_Aiming))
	{
		SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_Ang = 0.0f;
		m_StartY = 0;
		m_NowY = 0;
		m_First = true;
	}
	if ((Input::GetKeyPress(VK_RBUTTON)) && (!m_Aiming))
	{
		m_Aiming = true;
	}



	if ((!Input::GetKeyPress(VK_RBUTTON))||(m_ReCount>0))
	{
		m_Aiming = false;
		SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_First = true;
	}



	if (m_Aiming)
	{
		POINT pt;
		GetCursorPos(&pt);
		m_NowY = pt.y;

		float mc = (m_StartY - m_NowY) * 0.1f;
		if (mc<0)
		{	mc *= -1;		}

		if (m_NowY < m_StartY)
		{

			//m_Ang += KANDO;
			m_Ang += KANDO * (mc);
		}
		if (m_NowY == m_StartY)
		{
			//SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
		if (m_NowY > m_StartY)
		{
			//m_Ang -= KANDO;
			m_Ang -= KANDO * (mc);
		}
		m_StartY = m_NowY;

		if (m_Ang > GUN_ANG)
		{	m_Ang = GUN_ANG;		}
		if (m_Ang < -GUN_ANG)
		{	m_Ang = -GUN_ANG;		}

		if (m_First)
		{
			m_Ang = 0.0f;
			m_First = false;
			m_SEAim->Play(false);
		}

		if ((m_Type==TYPE_AR)||(m_Type == TYPE_SMG) || (m_Type == TYPE_LMG))
		{
			if ((Input::GetKeyPress(VK_LBUTTON)) && (m_Count <= 0) && (m_Ammo > 0))
			{
				int a = m_Ang * 0.9f;
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition(), CreateAng(a), TYPE_KUNAI);
				ShotFlag = true;
			}
		}

		if (m_Type == TYPE_DMR)
		{	
			if ((Input::GetKeyTrigger(VK_LBUTTON)) && (m_Count <= 0) && (m_Ammo > 0))
			{
				int a = m_Ang * 0.9f;
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition(), CreateAng(a),TYPE_KUNAI);
				ShotFlag = true;
			}
		}

		if (m_Type == TYPE_SHURI)
		{
			if ((Input::GetKeyPress(VK_LBUTTON)) && (m_Count <= 0) && (m_Ammo > 0))
			{
				int a = m_Ang * 0.9f;
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition(), CreateAng(a),TYPE_SHURIKEN);
				ShotFlag = true;
			}
		}

		if (m_Type == TYPE_SG)
		{
			if ((Input::GetKeyTrigger(VK_LBUTTON)) && (m_Count <= 0) && (m_Ammo > 0))
			{
				int a = m_Ang * 0.9f;
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition(), CreateAng(a), TYPE_SHURIKEN);
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition() + D3DXVECTOR3(0.0f,  0.5f, 0.0f), CreateAng(a), TYPE_SHURIKEN);
				scene->AddGameObject<EntityPBullet>(1)->SetBullet(player->GetPosition() + D3DXVECTOR3(0.0f, -0.5f, 0.0f), CreateAng(a), TYPE_SHURIKEN);
				ShotFlag = true;
			}
		}


	}
	else
	{	m_Ang = 0.0f;	}

	if (ShotFlag)
	{
		m_Count = m_Rate;
		m_Ammo--;
		if (m_Ammo == 0)
		{
			m_ReCount = m_ReTime;
		}
		m_SEShot->Play(false);
	}

	if (m_ReCount == 0)
	{	m_Ammo = m_Maga;	}

	m_ReCount--;
	m_Count--;
	
	//スコア表示
	m_ScoreAmmo ->SetScore(m_Ammo);
	m_ScoreReCnt->SetScore(m_ReCount);
	m_ScoreAngle->SetScore(m_Ang*100.0f);
}

void WEAPON::Draw()
{
	if(m_Aiming)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(-LASER_SIZEX, LASER_SIZEY, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(LASER_SIZEX, LASER_SIZEY, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(-LASER_SIZEX, -LASER_SIZEY, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(11.0f, 0.0f, 0.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(LASER_SIZEX, -LASER_SIZEY, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダ設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//カメラのビューマトリクス取得
		Scene* scene = Manager::GetScene();
		CAMERA* camera = scene->GetGameObject<CAMERA>();
		EntityPlayer* player = scene->GetGameObject<EntityPlayer>();
		D3DXMATRIX view = camera->GetViewMatrix();

		//レーザーの角度を生成
		float ang = m_Ang;
		if (m_Ang >= 0.43f)
		{	ang = 0.43f;		}
		if (m_Ang <= -0.43f)
		{	ang = -0.43f;		}
		m_Rotation = D3DXVECTOR3(0.0f, 0.0f, ang);	
		//左向きの時
		if (player->GetLeft())
		{	m_Rotation.z = (ang + 3.15f) * -1.0f;		}

		{
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



	}
}

D3DXVECTOR3 WEAPON::CreateAng(float ang)
{
	Scene* scene = Manager::GetScene();
	EntityPlayer* player = scene->GetGameObject<EntityPlayer>();

	float a = ang;
	float b = ang;
	if (a < 0.0f)
	{
		//a = 0.48f - (m_Ang * -1.0f);
		a *= -1.0f;
	}

	D3DXVECTOR3 ang3 = D3DXVECTOR3(1.0f - a, m_Ang, 0.0f);
	if (player->GetLeft())
	{
		ang3.x *= -1.0f;
	}
	return ang3;
}
