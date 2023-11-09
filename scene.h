#pragma once
#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>

#define LAYER	(5)

class Scene
{

protected:
	std::list<GameObject*> m_GameObject[LAYER];	//STLのリスト構造

public:
	virtual void Init(){}

	virtual void Uninit()
	{
		for (int i = 0; i < LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])	//範囲forループ
			{	gameObject->Uninit();	delete gameObject;	}

			m_GameObject[i].clear();	//リスト構造の削除
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])	//範囲forループ
			{	gameObject->Update();	}

			m_GameObject[i].remove_if([](GameObject* object)
				{return object->Destroy(); });
		}
	}

	void Draw()
	{
		for (int i = 0; i < LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])	//範囲forループ
			{	gameObject->Draw();		}
		}
	}

	

	template <typename T>	//テンプレート関数
	T* AddGameObject(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}


	template <typename T>	//テンプレート関数
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}


	template <typename T>	//テンプレート関数
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < LAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}



};