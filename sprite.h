#pragma once
#include "component.h"
//#include "main.h"

class SPRITE : public COMPONENT
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

public:

	void Init(float x, float y, float w, float h, const char* TextureName);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR color) { m_Color = color; }

};