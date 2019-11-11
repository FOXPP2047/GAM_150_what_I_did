/*******************************************************************************
filename     EmitterData.c
author       Minjeong.Kim, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
To make various particle effects, some datas (colors and position) specified
as enum.

*******************************************************************************/
#include "EmitterData.h"
#include "ParticleSystem.h"

/******************************************************************************/
/*!
Set minimum and maximum position and color variation of emitter.

\param id
id to verify the type of emitter

\param pCur
a pointer to emitter object to set the values according to type id.

*/
/******************************************************************************/
void SetPositionAndColor(EmitterType id, Emitter* pCur)
{
	WEVec2 posMin;
	WEVec2 posMax;
	WEVec2 botLeft;
	WEVec2 topRight;
	char minColor[4];
	char maxColor[4];
	int i;

	//according to ID, the values are changed
	switch (id)
	{
	case PS_Spark:

		posMin.x = -0.1f;
		posMin.y = -0.1f;
		posMax.x = +0.3f;
		posMax.y = +0.3f;

		/*alpha, blue, green, red*/
		minColor[0] = 254;
		minColor[1] = 100;
		minColor[2] = 100;
		minColor[3] = 100;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 150;
		maxColor[3] = 150;

		break;
	case PS_Fire:

		posMin.x = -0.1f;
		posMin.y = -0.1f;
		posMax.x = 0.1f;
		posMax.y = 0.1f;

		/*alpha, blue, green, red*/
		minColor[0] = 254;
		minColor[1] = 240;
		minColor[2] = 240;
		minColor[3] = 240;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 255;
		maxColor[3] = 255;

		break;
	case PS_CoreDamaged:

		posMin.x = -2.f;
		posMin.y = -2.f;
		posMax.x = 2.f;
		posMax.y = 2.f;

		/*alpha, blue, green, red*/
		minColor[0] = 100;
		minColor[1] = 100;
		minColor[2] = 100;
		minColor[3] = 100;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 150;
		maxColor[3] = 150;

		break;
	case PS_Warn:

		posMin.x = -0.1f;
		posMin.y = -0.1f;
		posMax.x = 0.1f;
		posMax.y = 0.1f;

		/*alpha, blue, green, red*/
		minColor[0] = 254;
		minColor[1] = 0;
		minColor[2] = 0;
		minColor[3] = 100;

		maxColor[0] = 255;
		maxColor[1] = 100;
		maxColor[2] = 100;
		maxColor[3] = 255;

		break;
	case PS_Explore:

		posMin.x = -3.f;
		posMin.y = -3.f;
		posMax.x = 4.f;
		posMax.y = 4.f;

		/*alpha, blue, green, red*/
		minColor[0] = 150;
		minColor[1] = 100;
		minColor[2] = 100;
		minColor[3] = 100;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 255;
		maxColor[3] = 255;

		break;
	case PS_BackGround:
		botLeft = WEGraphicsGetWorldBotLeft();
		topRight = WEGraphicsGetWorldTopRight();

		posMin.x = botLeft.x;
		posMin.y = botLeft.y;
		posMax.x = topRight.x;
		posMax.y = topRight.y;

		/*alpha, blue, green, red*/
		minColor[0] = 50;
		minColor[1] = 100;
		minColor[2] = 100;
		minColor[3] = 100;

		maxColor[0] = 150;
		maxColor[1] = 150;
		maxColor[2] = 150;
		maxColor[3] = 150;

		break;
	case PS_SlowPlanet:

		posMin.x = -1.f;
		posMin.y = -1.f;
		posMax.x = 1.f;
		posMax.y = 1.f;

		/*alpha, blue, green, red*/
		minColor[0] = 254;
		minColor[1] = 200;
		minColor[2] = 200;
		minColor[3] = 0;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 255;
		maxColor[3] = 50;

		break;
	case PS_PoisonPlanet:
		posMin.x = -3.f;
		posMin.y = -3.f;
		posMax.x = 4.f;
		posMax.y = 4.f;

		/*alpha, blue, green, red*/
		minColor[0] = 200;
		minColor[1] = 0;
		minColor[2] = 200;
		minColor[3] = 0;

		maxColor[0] = 255;
		maxColor[1] = 70;
		maxColor[2] = 255;
		maxColor[3] = 70;
		break;
	case PS_Bomb:
		posMin.x = -1.f;
		posMin.y = -1.f;
		posMax.x = 1.f;
		posMax.y = 1.f;

		/*alpha, blue, green, red*/
		minColor[0] = 200;
		minColor[1] = 0;
		minColor[2] = 50;
		minColor[3] = 200;

		maxColor[0] = 255;
		maxColor[1] = 150;
		maxColor[2] = 100;
		maxColor[3] = 255;
		break;
	default:

		posMin.x = -3.f;
		posMin.y = -3.f;
		posMax.x = 4.f;
		posMax.y = 4.f;

		/*alpha, blue, green, red*/
		minColor[0] = 254;
		minColor[1] = 100;
		minColor[2] = 100;
		minColor[3] = 100;

		maxColor[0] = 255;
		maxColor[1] = 255;
		maxColor[2] = 150;
		maxColor[3] = 150;

		break;
	}


	//set them.
	pCur->minPos = posMin;
	pCur->maxPos = posMax;

	for (i = 0; i < 4; ++i)
	{
		pCur->maxColor[i] = maxColor[i];
		pCur->minColor[i] = minColor[i];
	}
}