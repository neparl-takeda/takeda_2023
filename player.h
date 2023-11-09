#pragma once

#include "gameObject.h"
#include "model.h"

class PLAYER : public GameObject
{

private:

	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};

	class Audio* m_ShotSE{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	bool GetMoving();
};

