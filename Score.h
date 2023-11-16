#pragma once
#include "gameObject.h"

enum SCORE_TYPE
{
	TYPE_SCORE = 0,
	TYPE_AMMO,
	TYPE_RELOAD,
	TYPE_ANGLE_D,
};

class SCORE : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR4 m_NumCol[4]{};
	D3DXVECTOR4 m_ColA[4]{};
	D3DXVECTOR4 m_ColB[4]{};

	SCORE_TYPE m_Type{};

	int m_Score{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddScore(int up) { m_Score += up; }
	void SetScore(int num) { m_Score = num; }
	void SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR4 colA, D3DXVECTOR4 colB, SCORE_TYPE type);

	SCORE_TYPE GetType() { return m_Type; }
};