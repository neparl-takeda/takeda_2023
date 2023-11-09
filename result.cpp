#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "result.h"

#include "camera.h"
#include "polygon2D.h"
#include "ResultLogo.h"

#include "input.h"
#include "title.h"
#include "audio.h"

#include "fade.h"


void SCENE_RESULT::Init()
{
	AddGameObject<RESULTLOGO>(2);

	Audio* bgm;
	bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	bgm->Load("asset\\sound\\BGM_Result.wav");
	bgm->Play(true);

	m_Fade = AddGameObject<Fade>(3);
	m_Fade ->SetFade(true);
}

void SCENE_RESULT::Update()
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
		Manager::SetScene<SCENE_TITLE>();
	}
}