#pragma once

#include "component.h"
#include <list>

class GameObject
{
protected:

	bool		m_Destroy = false;

	D3DXVECTOR3 m_Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	class DrawCollision* m_DrawColl;

	std::list<COMPONENT*> m_Component;

public:

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{	return false;	}
	}

	virtual void Init()	 
	{
	
	}

	virtual void Uninit()
	{
		for (COMPONENT* component : m_Component)	//範囲forループ
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();	//リスト構造の削除
	}

	virtual void Update()
	{
		for (COMPONENT* component : m_Component)	//範囲forループ
		{
			component->Update();
		}
	}

	virtual void Draw()	 
	{
		for (COMPONENT* component : m_Component)	//範囲forループ
		{
			component->Draw();
		}
	}

	void SetPosition(D3DXVECTOR3 pos)
	{	m_Position = pos;	}

	void SetScale(D3DXVECTOR3 scale)
	{	m_Scale = scale;	}

	void SetRotation(D3DXVECTOR3 rot)
	{	m_Scale = rot;	}

	void SetTransform(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scale)
	{
		m_Position = pos;
		m_Rotation = rot;
		m_Scale  = scale;
	}

	D3DXVECTOR3 GetPosition()
	{	return m_Position;	}

	D3DXVECTOR3 GetRotation()
	{	return m_Rotation;	}

	D3DXVECTOR3 GetScale()
	{	return m_Scale;		}

	D3DXVECTOR3 GetForward()	//前方向ベクトルの取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetRight()	//前方向ベクトルの取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}


	template <typename T>
	T* AddComponent()
	{
		COMPONENT* component = new T();
		m_Component.push_back(component);
		component->Init();

		return (T*)component;
	}
	
};
