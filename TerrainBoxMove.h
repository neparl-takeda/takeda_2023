#pragma once

#include "gameObject.h"
#include "model.h"

class TerrainBoxMove : public GameObject
{

private:

	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int		m_Count	= 0;
	int		m_Add	= 1;
	int		m_Max	= 10;
	float	m_Spd	= 0.01f;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetParam(D3DXVECTOR3 pos, D3DXVECTOR3 scl, int max, float spd);
};

#pragma once
