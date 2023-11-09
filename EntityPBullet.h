#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"

static enum BULLET_TYPE
{
	TYPE_SHURIKEN = 0,
	TYPE_KUNAI=1,
};

class EntityPBullet : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};

	static ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_Direction{};
	int  m_Type = TYPE_SHURIKEN;
	int  m_Count{};
	bool m_Moving{};
	bool m_Destroy{};

	float m_gravity=0.02f;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 ang, BULLET_TYPE type) { m_Position = pos; m_Direction = ang; m_Type = type; }

	static void Load();
	static void Unload();

};
