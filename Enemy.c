/*******************************************************************************
filename     Enemy.c
author       yongmin.cho
email				 qlxhdl55@gmail.com (yongmin.cho)
project name Mediocre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is for Initialize and Update the enemy.

*******************************************************************************/
#include "Enemy.h"
#include <math.h> //atan2f
//This Scope is for Curve enemy. 
//if distance between Core and Enemy is less than this Scope, Update like Normal.
const int Scope = 200;

/*******************************************************************************
Function: YM_InitCurveEnemy

Description: Initialize the Enemy having Curve pattern.

Inputs: pEnemy - use for Enemy struct.

Outputs: None.
*******************************************************************************/
void YM_InitCurveEnemy(Enemy* pEnemy)
{
  //subVec is for vector what position of Enemt minus destination of enemy.
	WEVec2 subVec;
	//This is for Normalize the subVec.
	WEVec2 chaseDirection = { 0, 0 };
	//Initialize the timer to zero.
	pEnemy->timer = 0;
	//minus the position to destination.
	WEVec2Sub(&subVec, &pEnemy->destination, &pEnemy->physical.pos);
	//Initialize the rot using the atan2f property.
	pEnemy->rot = atan2f(subVec.y, subVec.x);
	
	if (!(subVec.x == 0 && subVec.y == 0))
		WEVec2Normalize(&chaseDirection, &subVec);
	//Vector set vel and acc.
	WEVec2Set(&pEnemy->vel, cosf(pEnemy->rot) - sinf(pEnemy->rot), sinf(pEnemy->rot) + cosf(pEnemy->rot));
	WEVec2Set(&pEnemy->acc, 0.f, 0.f);
}

/*******************************************************************************
Function: YM_NormalUpdateEnemy

Description: Update the Enemy having Normal pattern.

Inputs: pEnemy - use for Enemy struct.

Outputs: None.
*******************************************************************************/
void YM_NormalUpdateEnemy(Enemy* pEnemy, float dt)
{
	//subVec is for vector what position of Enemt minus destination of enemy.
	WEVec2 subVec;
	//This is for Normalize the subVec.
	WEVec2 chaseDirection = { 0, 0 };

	//minus the position to destination.
	WEVec2Sub(&subVec, &pEnemy->destination, &pEnemy->physical.pos);
	//Initialize the rot using the atan2f property.
	pEnemy->rot = atan2f(subVec.y, subVec.x);

	if (subVec.x != 0 || subVec.y != 0)
		WEVec2Normalize(&chaseDirection, &subVec);
	//vel equal to chasDirection multiply with speed. 
	pEnemy->vel.x = chaseDirection.x * pEnemy->speed;
	pEnemy->vel.y = chaseDirection.y * pEnemy->speed;
	//pos equal to plused vel.
	pEnemy->physical.pos.x += pEnemy->vel.x * dt;
	pEnemy->physical.pos.y += pEnemy->vel.y * dt;
}

/*******************************************************************************
Function: YM_CurveUpdateEnemy

Description: Update the Enemy having Curve pattern.

Inputs: pEnemy - use for Enemy struct.

Outputs: None.
*******************************************************************************/
void YM_CurveUpdateEnemy(Enemy* pEnemy, float dt)
{
	//subVec is for vector what position of Enemt minus destination of enemy.
	WEVec2 subVec;
	//This is for Normalize the subVec.
	WEVec2 chaseDirection = { 0, 0 };

	//Condition more than Scope
	if (powf((pEnemy->physical.pos.x - pEnemy->destination.x), 2) + powf((pEnemy->physical.pos.y - pEnemy->destination.y), 2) > Scope)
	{
		//plused dt to pEnemt->timer each per frame.
		pEnemy->timer += dt;
		//acc set
		pEnemy->acc.x = - sinf(pEnemy->rot) * (-sinf(pEnemy->timer));
		pEnemy->acc.y = + cosf(pEnemy->rot) * (-sinf(pEnemy->timer));
		//Change the Enemy velocity by plusing the accleration each per frame.
		pEnemy->vel.x += pEnemy->acc.x * dt;
		pEnemy->vel.y += pEnemy->acc.y * dt;
	}

	//Condition less than Scope
	else if (powf((pEnemy->physical.pos.x - pEnemy->destination.x), 2) + powf((pEnemy->physical.pos.y - pEnemy->destination.y), 2) <= Scope)
	{
		//minus the position to destination.
		WEVec2Sub(&subVec, &pEnemy->destination, &pEnemy->physical.pos);
		//Initialize the rot using the atan2f property.
		pEnemy->rot = atan2f(subVec.y, subVec.x);

		if (subVec.x != 0 || subVec.y != 0)
			WEVec2Normalize(&chaseDirection, &subVec);
		//Change the Enemy velocity by plusing the chaseDirection.
		pEnemy->vel.x = chaseDirection.x;
		pEnemy->vel.y = chaseDirection.y;
	}
	//Move the Enemy position by plusing the velocity each per frame.
	pEnemy->physical.pos.x += pEnemy->vel.x * pEnemy->speed * dt;
	pEnemy->physical.pos.y += pEnemy->vel.y * pEnemy->speed * dt;
}