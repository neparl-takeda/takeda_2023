#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"

//#define EFFECT_TYPES (2)


class EntityEffect : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};

	static ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_Direction{};
	int  m_Count{};
	bool m_Moving{};
	bool m_Destroy{};

	int m_TexX, m_TexY, m_Max{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, int x, int y, int m);

	static void Load();
	static void Unload();
};
