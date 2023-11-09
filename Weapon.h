#pragma once
//#include <D3DX10.h>
//#include <D3DX11.h>
#include "gameObject.h"

//#define EFFECT_TYPES (2)

static enum WEAPON_TYPE
{
	TYPE_SMG=0,
	TYPE_LMG,
	TYPE_AR,
	TYPE_DMR,
	TYPE_SG,
	TYPE_SHURI
};

class WEAPON : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = {};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_Direction{};

	class SCORE* m_ScoreAmmo;
	class SCORE* m_ScoreReCnt;
	class SCORE* m_ScoreAngle;

	class Audio* m_SEShot;
	class Audio* m_SEAim;

	int  m_Rate{};
	int  m_Count{};
	
	int  m_Ammo{};
	int  m_Maga{};

	int  m_ReCount{};
	int  m_ReTime{};

	int  m_Damage{};

	int m_StartY, m_NowY{};

	bool m_Moving{};
	bool m_Destroy{};
	bool m_Aiming{};
	bool m_First{};

	bool m_Fire{};

	float m_Ang{};

	WEAPON_TYPE m_Type{};


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetWeapon();
	bool GetAiming() { return m_Aiming; }
	D3DXVECTOR3 CreateAng(float ang);
};
