/*******************************************************************************
filename     IronArrow.c
author       yongmin.cho
email        qlxhdl55@gmail.com (Yongmin.Cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This file is Make Iron Arrow Bullet, and initialize Iron planet, add Iron Arrow
Bullet.

*******************************************************************************/
#include "IronArrow.h"
#include <math.h> // atan2f
#include "MJBullets.h" //makeBullet
/*******************************************************************************
Function: YMInitilaizeIron

Description: Initialize the Iron planet and IronArrowHead.

Inputs: pIron - use for Iron struct.
texture - texture of IronArrow.

Outputs: None.
*******************************************************************************/
void YMInitilaizeIron(Iron* pIron, int texture)
{
	//Initialize the IronArrowHead.
	pIron->pIronArrowHead = 0;
	//Initialize the texture of IronArrow
	pIron->textureID = texture;
	//Intiailize the base.Timer to zero.
	pIron->base.Timer = 0.f;
}

/*******************************************************************************
Function: YMAddIronArrow

Description: Add the IronArrow and Initialize the IronArrow.

Inputs: pIron - use for Iron struct.
direction - This parameter for check IronArrow direction.

Outputs: None.
*******************************************************************************/
void YMAddIronArrow(Iron* pIron, WEVec2 direction)
{
	//Declare the pCurIron for substituting head of pIronArrow.
	IronArrow* pCurIron = pIron->pIronArrowHead;
	//Declare for newIronArrow
	IronArrow* pNewIronArrow = 0;

	//if pCurIron is not NULL, loop this.
	while (pCurIron)
	{
		//if pCurIron->onFire is zero, substitute pCurIron to pNewIronArrow. 
		if (!pCurIron->onFire)
		{
			pNewIronArrow = pCurIron;
			break;
		}
		//And pass them.
		pCurIron = pCurIron->pNext;
	}

	//if pCUrIron is NULL, execute this.
	if (!pCurIron)
	{
		//MakeBullet.
		pNewIronArrow = MakeBullet();
		//handover head of slugBullet to next of pNewSlugBullet.
		pNewIronArrow->pNext = pIron->pIronArrowHead;
		//And Substitute to head of SlugBullet.
		pIron->pIronArrowHead = pNewIronArrow;
	}

	//Initialize the IronArrow.
	pNewIronArrow->trs.scale.x = 8.f;
	pNewIronArrow->trs.scale.y = 0.5f;
	pNewIronArrow->speed = 50.f;
	pNewIronArrow->onFire = 1;
	pNewIronArrow->trs.pos = *pIron->base.pOwner;
	WEVec2Normalize(&direction, &direction);
	//decide the IronArrow velocity what inculding direction and size.
	WEVec2Scale(&pNewIronArrow->vel, &direction, pNewIronArrow->speed);
	pNewIronArrow->rot = atan2f(direction.y, direction.x);
	//Initailize the angle of the planet.
	*pIron->base.pRot = pNewIronArrow->rot;
	//Reintialize the base.Timer to zero.
	pIron->base.Timer = 0.f;
	//Reinitilize the state to Working.
	pIron->base.state = Working;
}

/*******************************************************************************
Function: YMUpdateIronShot

Description: Update the IronArrow and IronPlanet.

Inputs: pIron - use for Iron struct.
pCurMap - This parameter is for knowing entire map size.
dt - This parameter is for frame seconds for using per frame.

Outputs: None.
*******************************************************************************/
void YMUpdateIronShot(Iron* pIron, MapInfo* pCurMap, float dt)
{
		//Declare the pCurArrow to substitute pIronArrowHead.
	IronArrow* pCurArrow = pIron->pIronArrowHead;
	//base.Timer plus dt(0.016) by per frame.
	pIron->base.Timer += dt;
	//if baseTimer more than duration make the Standingby State.
	if (pIron->base.Timer >= pIron->base.duration)
	{
		pIron->base.state = Standingby;
		*pIron->base.pActive = 0;
	}
	//Loop, if pCurArrow is not NULL.
	while (pCurArrow)
	{
			//And if pCurArrow->onFire exist, execute this.
		if (pCurArrow->onFire)
		{
				//Update the position by plusing the velocity each per frame.
			pCurArrow->trs.pos.x += pCurArrow->vel.x * dt;
			pCurArrow->trs.pos.y += pCurArrow->vel.y * dt;
			//if IronArrow position is not in the map, onFire make to zero.
			if (!IsInMap(pCurMap, pCurArrow->trs.pos.x, pCurArrow->trs.pos.y))
			{	
				pCurArrow->onFire = 0;
			}
		}
		//pass the next pCurArrow.
		pCurArrow = pCurArrow->pNext;
	}
}

/*******************************************************************************
Function: YMDrawIronArrow

Description: Draw the IronArrow.

Inputs: pIron - for using struct of Iron.

Outputs: None.
*******************************************************************************/
void YMDrawIronArrow(Iron* pIron)
{
	DrawBullet(pIron->pIronArrowHead, pIron->textureID);
}