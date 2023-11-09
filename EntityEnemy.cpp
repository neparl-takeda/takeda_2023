#include "main.h"
#include "renderer.h"
#include "EntityEnemy.h"
#include "EntityPBullet.h"
#include "EntityEffect.h"
#include "EntityShadow.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "box.h"
#include "cylinder.h"

//デバッグ用コリジョン表示
#include "DrawCollision.h"


#define PLAYER_SIZE (2.0f)
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void EntityEnemy::Init()
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
		"asset\\texture\\lifeA.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Count = 0;
	m_Moving = false;
	m_BulletAng = D3DXVECTOR2(0.0f, 0.0f);
	m_Left = false;

	m_Scale *= 2.0f;

	m_Hp = 6;

	//デバッグ用コリジョン表示
	Scene* scene = Manager::GetScene();
	m_DrawColl = scene->AddGameObject<DrawCollision>(3);
	m_Shadow = AddComponent<EntityShadow>();
}

void EntityEnemy::Uninit()
{
	GameObject::Uninit();
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void EntityEnemy::Update()
{
	GameObject::Update();
	D3DXVECTOR3 oldPosition = m_Position;

	Scene* scene = Manager::GetScene();


	//衝突判定
	float groundHeight = 0.0f;


	//cylinder
	std::vector<CYLINDER*> cylinders = scene->GetGameObjects<CYLINDER>();
	for (CYLINDER* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();

		//groundHeight = position.y + scale.y / 2.0f;

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);
		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
		}
	}

	//box
	std::vector<BOX*> boxes = scene->GetGameObjects<BOX>();
	for (BOX* box : boxes)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale	 = box->GetScale();

		if (position.x - scale.x < m_Position.x &&
			m_Position.x < position.x + scale.x &&
			position.z - scale.z < m_Position.z &&
			m_Position.z < position.z + scale.z 
			)
		{
			if (m_Position.y < position.y + scale.y * 2.0f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}

		}
	}

	//タマとの当たり判定
	std::vector<EntityPBullet*> bullets = scene->GetGameObjects<EntityPBullet>();
	for (EntityPBullet* bullet : bullets)
	{
		D3DXVECTOR3 position = bullet->GetPosition();
		D3DXVECTOR3 scale	 = bullet->GetScale();
		if ((position.x + scale.x / 1 > m_Position.x - m_Scale.x / 1)&&
			(position.x - scale.x / 1 < m_Position.x + m_Scale.x / 1)&&
			(position.y + scale.y / 1 > m_Position.y - m_Scale.y / 1)&&
			(position.y - scale.y / 1 < m_Position.y + m_Scale.y / 1)
			)
		{	
			bullet->SetDestroy();
			m_Hp--;
			scene->AddGameObject<EntityEffect>(1)->SetEffect(position, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 8, 2, 16);
		}
	}



	//ジャンプ
	if (m_Position.y - m_Scale.y < groundHeight + 0.1f && m_Velocity.y < 0.1f)
	{
		if ((Input::GetKeyTrigger(VK_SPACE)))
		{
			m_Velocity.y = 0.35f;
		}
	}
	m_Position += m_Velocity;	//
	m_Velocity.y -= 0.015f;		//重力

	//接地
	if (m_Position.y - m_Scale.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight + m_Scale.y;
		m_Velocity.y = 0.0f;
	}

	if (m_Hp <= 0)
	{
		SetDestroy();
		m_DrawColl->SetDestroy();
	}

	D3DXVECTOR3 ShadowScale = m_Scale * (1.0f / (m_Position.y - groundHeight));
	m_Shadow->SetPosition(D3DXVECTOR3(m_Position.x, groundHeight + 0.1f, m_Position.z));
	m_Shadow->SetScale(ShadowScale);

	//デバッグ用コリジョン表示
	m_DrawColl->SetPosition(m_Position);
	m_DrawColl->SetScale(m_Scale);

	//デバッグ用
	//m_Position.x += 0.01f;

}

void EntityEnemy::Draw()
{
	GameObject::Draw();
	int d = 1;
	float x = m_Count % d * (1.0f / d);
	float y = m_Count / d * (1.0f / d);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / d), y);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / d));

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / d), y + (1.0f / d));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	CAMERA* camera = scene->GetGameObject<CAMERA>();
	D3DXMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

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

