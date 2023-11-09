#pragma once
#include "gameObject.h"

class CAMERA : public GameObject
{
private:
	
	D3DXVECTOR3 m_Target{};
	D3DXMATRIX  m_ViewMatrix{};
	D3DXVECTOR3 m_ShakeVe{};

	D3DXVECTOR3 m_Velocity{};

public:
	void Init()		override;
	void Update()	override;
	void Draw()		override;

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
	
	D3DXVECTOR3 CameraShake();
};