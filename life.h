#pragma once
#include "gameObject.h"

class LIFE : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR4 m_NumCol[3]{};

	int m_Life{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddLife(int up) { m_Life += up; }
	int  GetLife();
};
