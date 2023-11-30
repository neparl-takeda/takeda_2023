#pragma once

#include "main.h"

class AttackBox
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Scale;
	D3DXCOLOR	m_Color;

	int	 m_Count	= 0;
	bool m_Active	= false;

	int m_TexCount = 16;
	int m_TexX = 0;
	int m_TexY = 0;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetColor(D3DXCOLOR col);
	void SetActive(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot, int cnt);
	bool GetActive();
};