/*******************************************************************************
filename     Poison.c
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This file is for initialize POISON planet, and spray the particle.

*******************************************************************************/
#include "Poison.h"
#include <math.h> // atan2f
#include "ParticleSystem.h"//MJPSAddEmitter
/*******************************************************************************
Function: YMInitPoison

Description: Initialize the POISON planet.

Inputs: pPoison - use for POISON struct.

Outputs: None.
*******************************************************************************/
void YMInitPoison(POISON* pPoison)
{
	//Initailze the base.Timer to zero.
	pPoison->base.Timer = 0;
	//Initialize the SprayTiem to two.
	pPoison->SprayTime = 2.f;
	//And power intialize to 0.99.
	pPoison->power = 0.99f;
}

/*******************************************************************************
Function: YMSprayPoison

Description: This for spray the particle system every spray the poison.

Inputs: pPoison - use for POISON struct.
direction - for knowing the Target direction.

Outputs: None.
*******************************************************************************/
void YMSprayPoison(POISON* pPoison, WEVec2 direction)
{
	//region for create the particle.
	WEVec2 par;
	//Normalize the direction and for par.
	WEVec2 nor;
	//decide the POISON planet direction
	*pPoison->base.pRot = atan2f(direction.y, direction.x);
	//and nomalize for use the par.
	WEVec2Normalize(&nor, &direction);
	//Make the Working state.
	pPoison->base.state = Working;
	//Reintialize the base.Timer.
	pPoison->base.Timer = 0.f;
	//Sparayed equal to zero.
	pPoison->Sparayed = 0.f;
	//Initialize the par.
	par.x = pPoison->base.pOwner->x + nor.x * 3.f;
	par.y = pPoison->base.pOwner->y + nor.y * 3.f;
	//Sparayed the particle.
	MJPSAddEmitter(PS_PoisonPlanet, 100, par.x, par.y, *pPoison->base.pRot, PST_circle,
		1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 0.5f, 1.f);
}

/*******************************************************************************
Function: YMUpdatePoison

Description: This function for change the state of planet.

Inputs: pPoison - use for POISON struct.
dt - This parameter is for frame seconds for using per frame.

Outputs: None.
*******************************************************************************/
void YMUpdatePoison(POISON* pPoison, float dt)
{
	//As the planet state, execute.
	if (pPoison->base.state == Working)
		pPoison->Sparayed += dt;

	if (pPoison->Sparayed >= pPoison->SprayTime)
		pPoison->base.state = Recharging;

	if (pPoison->base.state == Recharging)
		pPoison->base.Timer += dt;

	if (pPoison->base.Timer >= pPoison->base.duration)
	{
		pPoison->base.state = Standingby;
		*pPoison->base.pActive = 0;
	}
}