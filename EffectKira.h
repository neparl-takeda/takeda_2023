#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"


class EFFECT : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	int m_TexX, m_TexY{};
	int m_Max{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
