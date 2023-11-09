#pragma once
#include "scene.h"

class Scene;	//ポインタ変数使いたい時は前方宣言だけでいける
				//includeしちゃうと循環参照になってまう

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