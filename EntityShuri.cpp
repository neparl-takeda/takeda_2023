#include "main.h"
#include "renderer.h"
#include "EntityPlayer.h"
#include "EntityShuri.h"
#include "EntityEffect.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "box.h"
#include "cylinder.h"

#include "DrawCollision.h"

//#include "ScoreDbg.h"

//#define BULLET_SIZE		(0.5f,0.5f,0.5f)
#define PL_DIST (30.0f)


//テクスチャ
ID3D11ShaderResourceView* EntityShuriken::m_Texture{};

void EntityShuriken::Load()
{
	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\bullet_Player.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}

void EntityShuriken::Unload()
{
	m_Texture->Release();
	//delete m_Texture;
}

void EntityShuriken::Init()
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



	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Count = 0;
	m_Moving = false;
	m_Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * 0.25f;

	//デバッグ用コリジョン表示
	Scene* scene = Manager::GetScene();
	m_DrawColl = scene->AddGameObject<DrawCollision>(3);
}

void EntityShuriken::Uninit()
{
	m_VertexBuffer->Release();


	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void EntityShuriken::Update()
{
	D3DXVECTOR3 oldPosition = m_Position;

	Scene* scene = Manager::GetScene();

	//cylinder
	std::vector<CYLINDER*> cylinders = scene->GetGameObjects<CYLINDER>();
	for (CYLINDER* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);
		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y - 0.0f)
			{
				m_Destroy = true;
			}
		}
	}

	//box
	std::vector<BOX*> boxes = scene->GetGameObjects<BOX>();
	for (BOX* box : boxes)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x - 0.0f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.0f &&
			position.z - scale.z - 0.0f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.0f
			)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Destroy = true;
			}
		}
	}

	m_Position += m_Direction * 0.5f;

	EntityPlayer* player = scene->GetGameObject<EntityPlayer>();
	if ((m_Position.y >= 25.0f) || (m_Position.y <= -25.0f) ||
		(m_Position.x >= player->GetPosition().x + PL_DIST) || (m_Position.x <= player->GetPosition().x - PL_DIST))
	{
		m_Destroy = true;
	}


	if (m_Destroy)
	{
		SetDestroy();
		m_Destroy = false;
		scene->AddGameObject<EntityEffect>(1)->SetEffect(oldPosition, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 8, 2, 16);
		return;
	}

	//デバッグ用コリジョン表示
	m_DrawColl->SetPosition(m_Position);
	m_DrawColl->SetScale(m_Scale);
	//m_Count++;
}

void EntityShuriken::Draw()
{
	int type = 3;

	float x = m_Count % 4 * (1.0f / 4);
	float y = m_Count / 1 * (1.0f / 1);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x + 0.25f * (type - 1), y);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + 0.25f * (type), y);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x + 0.25f * (type - 1), y + 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + 0.25f * (type), y + 1.0f);

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

	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, m_Direction.y);

	//マトリクス設定
//	D3DXMATRIX world, scale, rot, trans;
//	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
//	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
///*	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
//	world = scale * invView * trans;*/
//	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
//	world = scale * trans;
//	Renderer::SetWorldMatrix(&world);
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

