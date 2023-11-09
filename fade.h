#pragma once
#include "gameObject.h"

class Fade : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class SPRITE* m_Sprite{};
	float m_Alpha = 1.0f;

	bool  m_In			= true;
	bool  m_FinishedOut = false;
	bool  m_FinishedIn	= false;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetFade(bool fade)
	{ 
		m_FinishedOut = false;
		m_FinishedIn = false;
		m_In = fade; 
	}

	bool GetFinishedOut() { return m_FinishedOut; }
	bool GetFinishedIn()  { return m_FinishedIn; }
};