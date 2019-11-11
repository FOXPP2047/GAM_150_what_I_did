/*******************************************************************************
filename     Core.c
author       yongmin.cho
email        qlxhdl55@gmail.com
project name Meteorcre Defense
course       GAME150kr
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This includes function for the cores.
*******************************************************************************/
#include "Core.h"

/******************************************************************************/
/*!
		A function which initializes the core.
		Author : Cho Yong Min

	\param cCore
		A pointer to the core.
	\param modifier
		A pointer to the
	\param textureID
		A texture for the core.

	\return
		Nothing.
*/
/******************************************************************************/
void YM_InitCore(Core* pCore, const YH_COREMODIFIER* pModifier, int textureID)
{
	pCore->lmnt.pos.x = pModifier->pos.x;
	pCore->lmnt.pos.y = pModifier->pos.y;
	pCore->lmnt.scale.x = pModifier->scale.x;
	pCore->lmnt.scale.y = pModifier->scale.y;
	pCore->life = pModifier->life;
	pCore->rotation = 0.f;
	pCore->rotationSpeed = pModifier->rotationSpeed;
	pCore->closeup = 0;
	pCore->coreTexture = textureID;
}

/******************************************************************************/
/*!
		A function which updates the radian of the cores.
		Author : Cho Yong Min

	\param cores
		An array of cores.
	\param dt
		Delta time.
	\param coreNumber
		A number of cores.

	\return
		Nothing.
*/
/******************************************************************************/
void UpdateRotationCore(Core cores[], const float dt, const int coreNumber)
{
	int i = 0;

	for (i = 0; i < coreNumber; ++i)
	{
		cores[i].rotation += dt * cores[i].rotationSpeed;

		if (cores[i].rotation >= WE_MATH_PI * 2.f)
			cores[i].rotation = 0.f;
	}
}

/******************************************************************************/
/*!
		A function which draws the cores.
		Author : Cho Yong Min

	\param pCore
		A pointer to core.
	\param coreNumber
		A number of cores.

	\return
		Nothing.
*/
/******************************************************************************/
void YM_DrawCore(const Core* pCore, const int coreNumber)
{
	WEMtx44 mtx;
	int i = 0;

	for (i = 0; i < coreNumber; ++i)
	{
		WEGraphicsSetAttributeColor(0xFFFFFFFF);
		WEGraphicsSetTexture(pCore[i].coreTexture);

		WEMtx44MakeTransform(&mtx,
			pCore[i].lmnt.scale.x,
			pCore[i].lmnt.scale.y,
			pCore[i].rotation,
			pCore[i].lmnt.pos.x,
			pCore[i].lmnt.pos.y,
			0);
		WEGraphicsDraw(&mtx);
	}
}
