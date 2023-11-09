#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "audio.h"

Scene* Manager::m_Scene{}; //静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{}; //静的メンバ変数は再宣言が必要

bool Manager::m_AllPose = false;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	
	SetScene<SCENE_TITLE>();

	
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (Input::GetKeyTrigger('P'))
	{
		if (!m_AllPose)
		{
			m_AllPose = true;
		}
		else
		{
			m_AllPose = false;
		}

	}

	if (m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}
	
	if (!m_AllPose)
	{
		m_Scene->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();
	
	m_Scene->Draw();

	Renderer::End();
}
