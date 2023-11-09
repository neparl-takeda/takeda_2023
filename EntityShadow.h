#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "component.h"

class EntityShadow : public COMPONENT
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetPosition(D3DXVECTOR3 pos) { m_Position	= pos; }
	void SetScale	(D3DXVECTOR3 scl) { m_Scale		= scl; }

};
