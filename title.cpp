#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "gameObject.h"
#include "title.h"

#include "titleLogo.h"
#include "fade.h"

#include "input.h"
#include "game.h"
#include "audio.h"


void SCENE_TITLE::Init()
{
	AddGameObject<TITLELOGO>(2);
	
	{
		Audio* bgm;
		bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
		bgm->Load("asset\\sound\\BGM_Title.wav");
		bgm->Play(true);
	}

	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFade(true);
}

void SCENE_TITLE::Update()
{
	Scene::Update();
	if (m_Fade->GetFinishedIn())
	{
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			m_Fade->SetFade(false);
		}
	}

	if (m_Fade->GetFinishedOut())
	{
		Manager::SetScene<SCENE_GAME>();
	}
}