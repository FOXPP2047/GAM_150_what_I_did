/*******************************************************************************
filename     Sieze.c
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This file is for Make Slug Bullet, 
													and initialize Sieze planet, add Slug Bullet.

*******************************************************************************/
#include "Sieze.h"
#include <math.h> // atan2f
#include "MJBullets.h"

/*******************************************************************************
Function: YMInitailizeSieze

Description: Initialize the Sieze planet and SlugBulletHead.

Inputs: pSieze - use for Sieze struct.
texture - texture of SlugBullet.

Outputs: None.
*******************************************************************************/
void YMInitializeSieze(Sieze* pSieze, int texture)
{
	//Initialize the SlugBulletHead
	pSieze->pSlugBulletHead = 0;
	//Initialzie the texture of SlugBullet.
	pSieze->textureID = texture;
  //Initialize the base.Timer to zero.
	pSieze->base.Timer = 0.f;
}

/*******************************************************************************
Function: YMAddSlugBullet

Description: Add the SlugBullet and Initialize the SlugBullet.

Inputs: pSieze - use for Sieze struct.
direction - This parameter for check SlugBullet direction.
destination - This parameter for Bullet destination.

Outputs: None.
*******************************************************************************/
void YMAddSlugBullet(Sieze* pSieze, WEVec2 direction, WEVec2 destination)
{
	//pCur is head of Slug.
	SlugBullet* pCur = pSieze->pSlugBulletHead;
	//next SlugBullet.
	SlugBullet* pNewSlugBullet = 0;

	//if pCur is not NULL, loop this.
	while (pCur)
	{
		//if pCur->onFire is zero, pNewSlugBullet equal pCur.
		if (!pCur->onFire)
		{
			pNewSlugBullet = pCur;
			break;
		}
		//And pass them.
		pCur = pCur->pNext;
	}

	//if pCur is NULL.
	if (!pCur)
	{
		//MakeBullet and substitute to pNewSlugBullet.
		pNewSlugBullet = MakeBullet();
		//handover head of slugBullet to next of pNewSlugBullet.
		pNewSlugBullet->pNext = pSieze->pSlugBulletHead;
		//And Substitute to head of SlugBullet.
		pSieze->pSlugBulletHead = pNewSlugBullet;
	}

	//Intitialize the SlugBullet.
	pNewSlugBullet->trs.scale.x = 5.f;
	pNewSlugBullet->trs.scale.y = 2.5f;
	pNewSlugBullet->speed = 50.f;
	pNewSlugBullet->onFire = 1;
	//Calculate the SlugBullet flyingTime.
	pNewSlugBullet->flyingTime = WEVec2Distance(pSieze->base.pOwner, &destination) / pNewSlugBullet->speed;
	pNewSlugBullet->trs.pos = *pSieze->base.pOwner;
	WEVec2Normalize(&direction, &direction);
	//decide the SlugBullet velocity what inculding direction and size.
	WEVec2Scale(&pNewSlugBullet->vel, &direction, pNewSlugBullet->speed);
	//and Intiailize the angle of the SlugBullet.
	pNewSlugBullet->rot = atan2f(direction.y, direction.x);
	//Initailize the angle of the planet.
	*pSieze->base.pRot = pNewSlugBullet->rot;
	//Reintialize the base.Timer to zero.
	pSieze->base.Timer = 0.f;
	//Reinitilize the State to Working
	pSieze->base.state = Working;
}

/*******************************************************************************
Function: YMUpdateSieze

Description: Update the SlugBullet and SlugPlanet.

Inputs: pSieze - use for Sieze struct.
dt - This parameter is for frame seconds for using per frame.

Outputs: None.
*******************************************************************************/
void YMUpdateSieze(Sieze* pSieze, float dt)
{
	//Declare the pCurSlugBullet to substitute pSlugBulletHead.
	SlugBullet* pCurSlugBullet = pSieze->pSlugBulletHead;
	//base.Timer plus dt(0.016) by per frame.
	pSieze->base.Timer += dt;
	//if baseTimer more than duration make the Standingby State.
	if (pSieze->base.Timer >= pSieze->base.duration)
	{
		pSieze->base.state = Standingby;
		//And Active equal zero.
		*pSieze->base.pActive = 0;
	}
	//Loop, if pCurSlugBullet is not NULL.
	while (pCurSlugBullet)
	{
			// And if pCurSlugBullet->onFire exist.
		if (pCurSlugBullet->onFire)
		{
			//Move the SlugBullet position by plusing the velocity each per frame.
			pCurSlugBullet->trs.pos.x += pCurSlugBullet->vel.x * dt;
			pCurSlugBullet->trs.pos.y += pCurSlugBullet->vel.y * dt;
			// And minus flyingTime minus dt by per frame.
			pCurSlugBullet->flyingTime -= dt;
		}
		//pass the next pCurSlugBullet.
		pCurSlugBullet = pCurSlugBullet->pNext;
	}
}

/*******************************************************************************
Function: YMBombSieze

Description: if SlugBullet flyingTime is equal to zero,
             burst the particle system.

Inputs: pSlugBullet - for using position of pSlugBullet.

Outputs: None.
*******************************************************************************/
void YMBombSieze(SlugBullet* pSlugBullet)
{
		//burst the particle system.
	MJPSAddEmitter(PS_Bomb, 1000, pSlugBullet->trs.pos.x, pSlugBullet->trs.pos.y, 0.f, PST_fire,
		3.f, 5.f, -WE_MATH_PI, WE_MATH_PI, 1.f, 3.2f, 0.5f, 1.5f);

}

/*******************************************************************************
Function: YMDrawSlugBullet

Description: Draw the SlugBullet.

Inputs: pSieze - for using struct of Sieze.

Outputs: None.
*******************************************************************************/
void YMDrawSlugBullet(Sieze* pSieze)
{
	DrawBullet(pSieze->pSlugBulletHead, pSieze->textureID);
}
