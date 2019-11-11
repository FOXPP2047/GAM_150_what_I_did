/*******************************************************************************
filename     Collisions.c
author       Minjeong.Kim, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:

Collision check and then give them<enemies> damage and effects.

Weapons and Enemies
Core and Enemies
Spike planet and Enemies

*******************************************************************************/
#include "Collisions.h"
#include "ParticleSystem.h"/*many particles Add*/
#include "Enemy.h"
#include "YHDefensePlanets.h"
#include "Generator.h"
#include "Weapon.h"
#include "Core.h"
#include "Sound.h"
#include "MJBullets.h"
#include "MJLaser.h"
#include "Freezing.h"
#include "Poison.h"
#include "Sieze.h"
#include "IronArrow.h"
#include <math.h>
 
/******************************************************************************/
/*!
Collision Check Enemies and Weapons

\param pEnemyHead
	a pointer to enemy head
\param pWmag
	a pointer to weapon manager
\param dt
	the usage of the time
*/
/******************************************************************************/
void MJCREnemyWeapon(Enemy* pEnemyHead, WeaponManager* pWmag, float dt)
{
	Bullet* pCur = 0;
	SlugBullet* pCurSlug = 0;
	IronArrow* pCurIronArrow = 0;
	Enemy* pCurEnemy = pEnemyHead;
	GUN* pCurGun = pWmag->pGunHead;
	LASER* pCurLaser = pWmag->pLaserHead;
	Freezing* pCurFreezing = pWmag->pFreezingHead;
	POISON* pCurPoison = pWmag->pPoisonHead;
	Sieze* pCurSieze = pWmag->pSiezeHead;
	Iron* pCurIron = pWmag->pIronHead;

	while (pCurEnemy)
	{
		//if enemy is alive and not yet freezed
		if (pCurEnemy->state == Alive && !pCurEnemy->slow)
		{
			pCurFreezing = pWmag->pFreezingHead;
			while (pCurFreezing)
			{
				//if freez weapon currently working now
				if (pCurFreezing->base.available && pCurFreezing->base.state == Working)
				{
					//there is collision of two of them 
					if (WEIntersectionPointCircle(&pCurEnemy->physical.pos, pCurFreezing->base.pOwner, pCurFreezing->base.range))
					{
						//do action
						pCurEnemy->speed *= pCurFreezing->power;
						pCurEnemy->slow = 1;
						break;
					}
				}
				pCurFreezing = pCurFreezing->pNext;
			}
		}
		pCurEnemy = pCurEnemy->pNext;
	}


	//every gun
	while (pCurGun)
	{
		pCur = pCurGun->pBulletHead;
		while (pCur)
		{
			//if bullet currently is working now
			if (pCur->onFire)
			{
				pCurEnemy = pEnemyHead;
				while (pCurEnemy)
				{	//if enemy is alive
					if (pCurEnemy->state != Dead)
						//there is collision of two of them 
						if (WEIntersectionPointRect(&pCur->trs.pos,
							&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x, pCurEnemy->physical.scale.y))
						{
							//do action
							pCurEnemy->life -= 1.f;
							//send particle
							MJPSAddEmitter(PS_Fire, 300, pCurEnemy->physical.pos.x, pCurEnemy->physical.pos.y, 0.f, PST_fire,
								1.f, 1.3f, -WE_MATH_PI, WE_MATH_PI, 1.f, 2.f, 0.5f, 0.8f);
							//bullet dead
							pCur->onFire = FALSE;
							break;
						}
					pCurEnemy = pCurEnemy->pNext;
				}
			}
			pCur = pCur->pNext;
		}
		pCurGun = pCurGun->pNext;
	}

	//every laser
	while (pCurLaser)
	{
		//is available and working now.
		if (pCurLaser->base.available && pCurLaser->base.state == Working)
		{
			pCurEnemy = pEnemyHead;
			while (pCurEnemy)
			{
				//if enemy is alive
				if (pCurEnemy->state != Dead)
					//there is collision of two of them 
					if (WEIntersectionCircleLine(&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x/2, &pCurLaser->point0, &pCurLaser->point1))
					{
						//do action
						pCurEnemy->life -= 5.f * dt * pCurEnemy->speed;
						//send effect particle
						MJPSAddEmitter(PS_Spark, 100, pCurEnemy->physical.pos.x, pCurEnemy->physical.pos.y, 0.f, PST_smoke,
							1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 0.3f, 0.5f);
					}
				pCurEnemy = pCurEnemy->pNext;
			}
		}
		pCurLaser = pCurLaser->pNext;
	}

	//every poison
	while (pCurPoison)
	{		//is available and working now.
		if (pCurPoison->base.available && pCurPoison->base.state == Working)
		{
			pCurEnemy = pEnemyHead;

			while (pCurEnemy)
			{	//if enemy is alive
				if (pCurEnemy->state != Dead)
				{
					//there is collision of two of them 
					if (WEIntersectionPointCircle(&pCurEnemy->physical.pos,
						pCurPoison->base.pOwner,
						pCurPoison->base.range))
						//give more damage
							pCurEnemy->infected += dt / 2.f;
					
				}
				pCurEnemy = pCurEnemy->pNext;
			}
		}
		pCurPoison = pCurPoison->pNext;
	}
	//every sieze
	while (pCurSieze)
	{
		pCurSlug = pCurSieze->pSlugBulletHead;
		
		while (pCurSlug)
		{ 
			if (pCurSlug->onFire)
			{
				pCurEnemy = pEnemyHead;
				//if it is reaching at the point to bomb
				if (pCurSlug->flyingTime <= 0.f)
				{
					//play sound
					Sound_Play(SOUND_SLUGBOMB);
					//Bomb!
					YMBombSieze(pCurSlug);
					pCurSlug->onFire = 0;
					while (pCurEnemy)
					{	//if enemy is alive and there is collision of two of them 
						if (pCurEnemy->state != Dead &&
							WEIntersectionCircleCircle(&pCurSlug->trs.pos, 10/*sieze bomb damage range*/, 
							&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x / 2))
						{
							pCurEnemy->life -= 5.f;
							if (!pCurEnemy->bombed)
								pCurEnemy->bombed = 1;
						}
						pCurEnemy = pCurEnemy->pNext;
					}
				}
			}
			pCurSlug = pCurSlug->pNext;
		}

		pCurSieze = pCurSieze->pNext;
	}
	//every Iron
	while (pCurIron)
	{
		pCurIronArrow = pCurIron->pIronArrowHead;

		while (pCurIronArrow)
		{	//is available and working now.
			if (pCurIronArrow->onFire)
			{
				pCurEnemy = pEnemyHead;

				while (pCurEnemy)
				{   //if enemy is alive and there is collision of two of them 
					if (pCurEnemy->state != Dead &&
						WEIntersectionCircleRect(&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x / 2,
						&pCurIronArrow->trs.pos, pCurIronArrow->trs.scale.x, pCurIronArrow->trs.scale.y))
					{
						MJPSAddEmitter(PS_Spark, 100, pCurEnemy->physical.pos.x, pCurEnemy->physical.pos.y, 0.f, PST_smoke,
							1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 0.3f, 0.5f);
						pCurEnemy->life -= 10.f * dt * pCurEnemy->speed;
					}
					pCurEnemy = pCurEnemy->pNext;
				}
			}
			pCurIronArrow = pCurIronArrow->pNext;
		}
		pCurIron = pCurIron->pNext;
	}
}
/******************************************************************************/
/*!
Collision Check Enemies and Core

\param pEnemyHead
a pointer to enemy head
\param pCore
a pointer to core
*/
/******************************************************************************/
void MJCREnemyCore(Enemy* pEnemyHead, Core* pCore)
{
	Enemy* pCurEnemy = pEnemyHead;

	while (pCurEnemy)
	{  //if enemy is alive and there is collision of two of them 
		if (pCurEnemy->state != Dead)
			if (WEIntersectionCircleRect(&pCore->lmnt.pos, pCore->lmnt.scale.x / 2.f,
				&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x, pCurEnemy->physical.scale.y))
				{
					MJPSAddEmitter(PS_CoreDamaged, 200, pCurEnemy->physical.pos.x, pCurEnemy->physical.pos.y, 0, PST_fire,
						5.f, 7.5f, -WE_MATH_PI, WE_MATH_PI, 1.f, 2.f, 0.9f, 1.45f);
					//core damaged, make enemy dead.
					--pCore->life;
					Sound_Play(SOUND_EXPLORE);
					AddNumDeadEnemy(-1);
					pCurEnemy->state = Dead;
				}

		pCurEnemy = pCurEnemy->pNext;
	}
}
/******************************************************************************/
/*!
Collision Check Enemies and Planet

\param pEnemyHead
a pointer to enemy head
\param pHead
a pointer to planets
\param dt
the usage of the time
*/
/******************************************************************************/
void MJCREnemyPlanet(Enemy* pEnemyHead, YH_DEFENSEPLANET* pHead, float dt)
{
	YH_DEFENSEPLANET* pReplica = pHead;
	Enemy* pCurEnemy;

	/*spike planet*/
	while (pReplica)
	{
		if (pReplica->planetID == PLANET_ONE)
		{
			pCurEnemy = pEnemyHead;
			while (pCurEnemy)
			{ //if enemy is alive and there is collision of two of them 
				if (pCurEnemy->state != Dead)
					if (WEIntersectionCircleRect(&pReplica->lmnt.pos, pReplica->lmnt.scale.x / 2.f,
						&pCurEnemy->physical.pos, pCurEnemy->physical.scale.x, pCurEnemy->physical.scale.y))
						{
							pCurEnemy->life -= 3.f * dt;
							MJPSAddEmitter(PS_Spark, 10, pCurEnemy->physical.pos.x, pCurEnemy->physical.pos.y, 0.f, PST_circle,
								1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 0.5f, 0.85f);
						}

				pCurEnemy = pCurEnemy->pNext;
			}
		}
		pReplica = pReplica->pNext;
	}
}