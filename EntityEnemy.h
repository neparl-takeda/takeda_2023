#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"

class EntityEnemy : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	int  m_Count{};
	bool m_Moving{};
	bool m_Left{};

	int m_Hp{};

	D3DXVECTOR2 m_BulletAng{};

	class EntityShadow* m_Shadow{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	bool GetLeft() { return m_Left; }
};
