#include "main.h"
#include "renderer.h"
#include "SkyDome.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "camera.h"

void SKY::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\Sky.obj");

	//m_Position	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(90.0f, 90.0f, 90.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void SKY::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_Model;
}

void SKY::Update()
{
	Scene* scene = Manager::GetScene();

	CAMERA* camera = scene->GetGameObject<CAMERA>();

	m_Position = camera->GetPosition() + D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	m_Rotation.y += 0.0001f;
}

void SKY::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//Renderer::SetCullingMode(CULL_MODE_FRONT);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
