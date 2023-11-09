#pragma once
#include "scene.h"

class SCENE_RESULT : public Scene
{
private:

	class Fade* m_Fade;

public:
	void Init()		override;
	void Update()	override;

};