#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "input.h"

#include "camera.h"
#include "field.h"
#include "polygon2D.h"
#include "Score.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"

#include "cylinder.h"
#include "box.h"
#include "TerrainBoxMove.h"

#include "Goal.h"
#include "life.h"
#include "audio.h"
#include "EntityPBullet.h"
#include "EntityEnemy.h"
#include "EntityEffect.h"
#include "SkyDome.h"
#include "DrawCollision.h"
#include "fade.h"

void SCENE_GAME::Init()
{
	//テクスチャ読み込み
	EntityPBullet::Load();
	EntityEffect::Load();

	AddGameObject<CAMERA>(0);
	AddGameObject<FIELD>(1);
	{		AddGameObject<EntityEnemy>(3)->SetPosition(D3DXVECTOR3(10.0f, 4.0f, 0.0f));	}
	{		AddGameObject<EntityEnemy>(3)->SetPosition(D3DXVECTOR3(35.0f, 0.0f, 0.0f));	}
	AddGameObject<PLAYER>(3)->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	//for (int i = 0; i < 9; i++)
	//{
	//	if (i < 5)
	//	{		AddGameObject<ENEMY>(1)->SetPosition(D3DXVECTOR3(-15.0f + i * 5.0f, 1.0f, 10.0f));		}
	//	else
	//	{		AddGameObject<ENEMY>(1)->SetPosition(D3DXVECTOR3(-12.5f + (i - 5) * 5.0f, 1.0f, 5.0f));	}

	//}

	{
		CYLINDER* cylinder = AddGameObject<CYLINDER>(1);
		cylinder->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 0.0f));
		cylinder->SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	}

	GOAL* goal = AddGameObject<GOAL>(1);
	goal->SetPosition(D3DXVECTOR3(60.5f, 7.0f, 0.0f));
	goal->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//ボックス
	{
		BOX* box = AddGameObject<BOX>(1);
		box->SetPosition(D3DXVECTOR3(25.0f, 0.0f, 0.0f));
		box->SetScale(D3DXVECTOR3(8.0f, 2.0f, 4.0f));
	}
	{
		BOX* box = AddGameObject<BOX>(1);
		box->SetPosition(D3DXVECTOR3(25.0f, 4.0f, 0.0f));
		box->SetScale(D3DXVECTOR3(4.0f, 2.0f, 4.0f));
	}
	{
		BOX* box = AddGameObject<BOX>(1);
		box->SetPosition(D3DXVECTOR3(50.0f, 0.0f, 0.0f));
		box->SetScale(D3DXVECTOR3(8.0f, 2.0f, 4.0f));
	}
	{
		BOX* box = AddGameObject<BOX>(1);
		box->SetPosition(D3DXVECTOR3(50.0f, 6.0f, 0.0f));
		box->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	}
	{
		TerrainBoxMove* box = AddGameObject<TerrainBoxMove>(1);
		box->SetPosition(D3DXVECTOR3(-25.0f, 0.0f, 0.0f));
		box->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	}

	AddGameObject<SKY>(1)->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	AddGameObject<SCORE>(3)->SetScore(D3DXVECTOR3(1200.0f,30.0f,0.0f), D3DXVECTOR3(50.0f,50.0f,0.0f), D3DXVECTOR4(0.4f,0.4f,0.4f,0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),TYPE_SCORE);
	AddGameObject<LIFE>(3);
	m_Fade = AddGameObject<Fade>(3);

	Audio* bgm;
	bgm=AddGameObject<GameObject>(0)->AddComponent<Audio>();
	bgm->Load("asset\\sound\\BGM_Game.wav");
	bgm->Play(true);

	//カーソル表示
	ShowCursor(false);
}

void SCENE_GAME::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<SCENE_TITLE>();
	}

	Scene* scene = Manager::GetScene();
	std::vector<GOAL*> goals = scene->GetGameObjects<GOAL>();
	for (GOAL* goal : goals)
	{
		bool ClearFlag = goal->GetClear();
		if (ClearFlag)
		{
			if (!m_Clear)
			{
				m_Fade->SetFade(false);
				m_Clear = true;
			}
		}
	}

	if ((m_Fade->GetFinishedOut())&&(m_Clear))
	{
		Manager::SetScene<SCENE_RESULT>();
	}

}

void SCENE_GAME::Uninit()
{
	Scene::Uninit();
	EntityPBullet::Unload();
	EntityEffect::Unload();
}