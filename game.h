#pragma once
#include "scene.h"

class SCENE_GAME : public Scene
{
private:

	class Fade* m_Fade;

	bool m_Clear = false;

public:
	void Init()		override;
	void Update()	override;
	void Uninit()	override;
};