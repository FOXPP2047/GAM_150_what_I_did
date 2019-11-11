/*******************************************************************************
filename     Core.h
author       yongmin.cho
email        qlxhdl55@gmail.com
project name Meteorcre Defense
course       GAME150kr
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
	This includes prototype for the functions in Core.h.
*******************************************************************************/
#ifndef CORE_H
#define CORE_H
#include "WEGameData.h"
#include "WarpEngine.h"
#include "YHObjects.h"
#include "GameObject.h"

typedef struct Core
{	
	Transform lmnt;
	int life;
	int closeup;
	int coreTexture;
	float rotation;
	float rotationSpeed;
}Core;

typedef struct
{
	WEVec2 pos;
	WEVec2 scale;
	int life;
	float rot;

	int orbitNumber;
	float orbitOffset;
	float betweenCoreFirstOrbit;
	float rotationSpeed;
}YH_COREMODIFIER;

void YM_InitCore(Core* pCore, const YH_COREMODIFIER* pModifier, int textureID);
void UpdateRotationCore(Core cores[], const float dt, const int coreNumber);
void YM_DrawCore(const Core* pCore, const int coreNumber);
#endif