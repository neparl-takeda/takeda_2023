#pragma once

#include "gameObject.h"
#include "model.h"

class BULLETVORTEX : public GameObject
{

private:

	D3DXVECTOR3			m_Velocity{};

	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

