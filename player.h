#pragma once

#include "gameObject.h"
#include "model.h"

class ComponentMarker;

class PLAYER : public GameObject
{

private:

	ComponentMarker* m_Marker;
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_RotationAtk{};

	class Audio* m_ShotSE{};

public:
	void Init()		override;
	void Uninit()	override;
	void Update()	override;
	void Draw()		override;
	bool GetMoving();
	void PlayerRotMath();
};

