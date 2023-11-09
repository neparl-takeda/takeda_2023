#pragma once

#include "gameObject.h"
#include "model.h"

class GOAL : public GameObject
{

private:

	bool m_Clear = false;

	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetClear() { return m_Clear; }
};

