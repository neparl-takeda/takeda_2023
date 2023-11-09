#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"

class EntityShuriken : public GameObject
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



public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetShuriken(D3DXVECTOR3 pos, D3DXVECTOR3 ang) { m_Position = pos; m_Direction = ang; }

	static void Load();
	static void Unload();

};
