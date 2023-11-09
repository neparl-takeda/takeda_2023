#pragma once
#include "scene.h"

class Scene;	//�|�C���^�ϐ��g���������͑O���錾�����ł�����
				//include�����Ⴄ�Əz�Q�ƂɂȂ��Ă܂�

class Manager
{
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	
	static bool m_AllPose;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; };

	template <typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}
};