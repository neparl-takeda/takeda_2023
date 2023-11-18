#pragma once
#include "component.h"

class ComponentMarker : public COMPONENT
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

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetRotation(D3DXVECTOR3 rot);
	void SetPosition(D3DXVECTOR3 pos);
	void SetColor	(D3DXCOLOR col);
};