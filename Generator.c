/*******************************************************************************
filename     Generator.c
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This function is to manage the Enemy.

*******************************************************************************/
#include "Generator.h"
#include <math.h> // sinf, cosf
#include "Coins.h"
#include "Weapon.h"
#include <stdlib.h> // malloc
#include "ParticleSystem.h" // MJPSAddEmitter
#include "Sound.h" // Sound_Play function

#define NumSmall 9 // Num of Enemy when Big Enemy change to small.

static int numDeadenemy; // Num of the dead Enemy.
// when create the small Enemy, for position changing.
static float radian = WE_MATH_TWO_PI/9.f;
// when create the small Enemy, radius of the poistion.
static float radius = 3.f;

/*******************************************************************************
Function: GetNumDeadEnemy

Description: return the static numDeadenemy.

Inputs: None.

Outputs: numDeadenemy - num of the Dead Enemy.
*******************************************************************************/
int GetNumDeadEnemy(void)
{
	return numDeadenemy;
}
/*******************************************************************************
Function: AddNumDeadEnemy

Description: add the numDeadEnemy.

Inputs: a - how many deadEnemy.

Outputs: None.
*******************************************************************************/
void AddNumDeadEnemy(int a)
{
	numDeadenemy += a;
}
/*******************************************************************************
Function: SetNumDeadEnemy

Description: Set the numDeadEnemy.

Inputs: a - Set the numDeadEnemy.

Outputs: None.
*******************************************************************************/
void SetNumDeadEnemy(int a)
{
	numDeadenemy = a;
}
/*******************************************************************************
Function: MakeEnemy

Description: MakeEnemy.

Inputs: None.

Outputs: newEnemy - return the Made Enemy.
*******************************************************************************/
static Enemy* MakeEnemy(void)
{
	//Make the newEnemy.
	Enemy* newEnemy = malloc(sizeof(Enemy));
	//Initialize the newEnemy->pNext equal to zero.
	newEnemy->pNext = 0;
	//return the newEnemy.
	return newEnemy;
}
/*******************************************************************************
Function: SturdyInit

Description: Initailize the Sturdy Enemy.

Inputs: pEnemy - using Enemy struct.

Outputs: None.
*******************************************************************************/
static void SturdyInit(Enemy* pEnemy)
{
	pEnemy->life =8.f;
	pEnemy->cost = 80;
	pEnemy->speed = 5.f;
	pEnemy->physical.scale.x = 2.f;
	pEnemy->physical.scale.y = 2.f;
}
/*******************************************************************************
Function: FastWeakInit

Description: Initailize the FastWeak Enemy.

Inputs: pEnemy - using Enemy struct.

Outputs: None.
*******************************************************************************/
static void FastWeakInit(Enemy* pEnemy)
{
	pEnemy->life = 4.0f;
	pEnemy->cost = 40;
	pEnemy->speed = 10.f;
	pEnemy->physical.scale.x = 1.5f;
	pEnemy->physical.scale.y = 1.5f;
}
/*******************************************************************************
Function: AlertInit

Description: Initailize the Alert Enemy.

Inputs: pEnemy - using Enemy struct.

Outputs: None.
*******************************************************************************/
static void AlertInit(Enemy* pEnemy)
{
	pEnemy->life = 6.0f;
	pEnemy->cost = 60;
	pEnemy->speed = 7.5f;
	pEnemy->physical.scale.x = 2.f;
	pEnemy->physical.scale.y = 2.f;
}
/*******************************************************************************
Function: BigInit

Description: Initailize the Big Enemy.

Inputs: pEnemy - using Enemy struct.

Outputs: None.
*******************************************************************************/
static void BigInit(Enemy* pEnemy)
{
	pEnemy->life = 15.f;
	pEnemy->cost = 150;
	pEnemy->speed = 2.f;
	pEnemy->physical.scale.x = 7.f;
	pEnemy->physical.scale.y = 7.f;
}
/*******************************************************************************
Function: SmallInit

Description: Initailize the Small Enemy.

Inputs: pEnemy - using Enemy struct.

Outputs: None.
*******************************************************************************/
static void SmallInit(Enemy* pEnemy)
{
	pEnemy->life = 3.f;
	pEnemy->cost = 30;
	pEnemy->speed = 5.f;
	pEnemy->physical.scale.x = 2.f;
	pEnemy->physical.scale.y = 2.f;
}
/*******************************************************************************
Function: EnemyManagerCreateEnemies

Description: Make the Enemy using linkedlist.

Inputs: ppEnemyHead - using Enemy struct.
whereToCreate - create poistion.
enemyProperty - decide the enemyProperty.
pattern - decide the enemyPattern.
destination - decide the enemy destination.

Outputs: None.
*******************************************************************************/
void EnemyManagerCreateEnemies(Enemy** ppEnemyHead, WEVec2 whereToCreate, EnemyProperty enemyProperty, EnemyPattern pattern, WEVec2 destination)
{
	//Initialize the pNew and decide to zero.
	Enemy* pNew = 0;
	//Initialize the pCur and decide to ppEnemyHead.
	Enemy* pCur = *ppEnemyHead;
	//if pCur is no NULL, loop this.
	while (pCur)
	{
			//if pCur->state equal Dead, pNew equal pCur.
		if (pCur->state == Dead)
		{
			pNew = pCur;
			break;
		}
		//And pass them.
		pCur = pCur->pNext;
	}
	//if pCur equal, MakeEnemy.
	if (pCur == 0)
	{
		pNew = MakeEnemy();
		//pNew->pNext equal to *ppEnemyHead.
		pNew->pNext = *ppEnemyHead;
		//*ppEnemyHead equal to pNew.
		*ppEnemyHead = pNew;
	}
	//Initialize the all of the porperty.
	pNew->physical.pos.x = whereToCreate.x;
	pNew->physical.pos.y = whereToCreate.y;
	pNew->slow = 0;
	pNew->bombed = 0;
	pNew->infected = 0;
	pNew->destination = destination;
	pNew->ID = enemyProperty;
	//As the enemyProperty, initialzie the different
	if (enemyProperty == Sturdy)
		SturdyInit(pNew);
	else if (enemyProperty == FastWeak)
		FastWeakInit(pNew);
	else if (enemyProperty == Alert)
		AlertInit(pNew);
	else if (enemyProperty == Big)
		BigInit(pNew);
	else if (enemyProperty == Small)
		SmallInit(pNew);

	//if pattern is equal to Curve, initialize the CurveEnemy. 
	if (pattern == Curve)
		YM_InitCurveEnemy(pNew);

	pNew->pattern = pattern;
	pNew->state = Alive;
	numDeadenemy++;
}

/*******************************************************************************
Function: BigToSmallUpdate

Description: Make the Enemy using linkedlist.

Inputs: ppEnemyHead - using Enemy struct.
pCur - using Enemy struct.

Outputs: None.
*******************************************************************************/
void BigToSmallUpdate(Enemy** ppEnemyHead, Enemy* pCur)
{
	//rad equal to radian.
	float rad = radian;
	//for loop
	int i;
	//when enemy dead, execute this.
	if (pCur->state == Dead)
	{
		//Small Enemy poistion.
		WEVec2 newPos;
		//make a particle
		MJPSAddEmitter(PS_Explore, 100, pCur->physical.pos.x, pCur->physical.pos.y, pCur->rot, PST_fire,
			1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 1.f, 2.f);
		//Create the Small enemies.
		for (i = 0; i < NumSmall; i++)
		{
			newPos.x = pCur->physical.pos.x + cosf(rad) * radius;
			newPos.y = pCur->physical.pos.y + sinf(rad) * radius;
			EnemyManagerCreateEnemies(ppEnemyHead, newPos, Small, Curve, pCur->destination);
			rad += radian;
		}
	}			
}

/*******************************************************************************
Function: EnemyManagerUpdateEnemies

Description: Update the Enemy.

Inputs: ppEnemyHead - using Enemy struct.
pWmag - using Enemy struct.
dt - This parameter is for frame seconds for using per frame.

Outputs: None.
*******************************************************************************/
void EnemyManagerUpdateEnemies(Enemy** ppEnemyHead, WeaponManager* pWmag, float dt)
{
  // pCur equal to ppEnemyHead.
	Enemy* pCur = *ppEnemyHead;
	// pCurPoison equal to zero.
	POISON* pCurPoison = 0;
	//if pWmag exist, pCurPoison equal to pPoisonHead.
	if (pWmag)
			pCurPoison = pWmag->pPoisonHead;
	//Loop, if pCur is not NULL.
	while (pCur)
	{
		//if Enemy State equal to Alive Update the Enemy as the pattern.
		if (pCur->state == Alive)
		{
			if (pCur->pattern == Normal)
				YM_NormalUpdateEnemy(pCur, dt);

			else if (pCur->pattern == Curve)
				YM_CurveUpdateEnemy(pCur, dt);

			//if Enemy is infected and pCurPoison exist, disgrace the life.
			if (pCurPoison && pCur->infected)
			{
				pCur->life *= pCurPoison->power * (pCurPoison->SprayTime / (int)(pCur->infected + pCurPoison->SprayTime));
				if (pCur->life <= WE_MATH_EPSILON)
					pCur->life = 0;
			}

			//if Enemy life less than zero, make Dead state.
			if (pCur->life <= 0)
			{
				pCur->state = Dead;
				//numDeadenemy minus.
				numDeadenemy--;
				//Sound_Play.
				Sound_Play(SOUND_EXPLORE);
				//plus the Coin.
				AddCoin(pCur->cost);
			}
			//if Enemy ID is Big, Update the BigToSmall Update.
			if (pCur->ID == Big)
			{
				BigToSmallUpdate(ppEnemyHead, pCur);
			}
		}
		//And Pass the next.
		pCur = pCur->pNext;
	}
}

/*******************************************************************************
Function: EnemyManagerDeleteEnemies

Description: Delete the Enemy.

Inputs: ppEnemyHead - using Enemy struct.

Outputs: None.
*******************************************************************************/
void EnemyManagerDeleteEnemies(Enemy** ppEnemyHead)
{
	//Enemy to Delete.
	Enemy* toDelete;
	//Previous list.
	Enemy* pPrev = 0;
	//Current Enemy List.
	Enemy* pCur = *ppEnemyHead;
	//Loop this, if pCUr is not Zero.
	while (pCur)
	{
		//if pCur->state is Dead, free the data.
		if (pCur->state == Dead)
		{
			if (pCur == *ppEnemyHead)
			{
				//free the toDelete.
				toDelete = pCur;
				*ppEnemyHead = pCur->pNext;
				free(toDelete);
				pCur = *ppEnemyHead;
			}

			else if (pPrev != 0)
			{
				//free the toDelete.
				toDelete = pCur;
				pPrev->pNext = pCur->pNext;
				free(toDelete);
				pCur = pPrev->pNext;
			}
		}
		//Enemy state is not Dead, previous substitute to pCur.
		if (pCur->state != Dead)
		{
			pPrev = pCur;
			//And Pass them.
			pCur = pCur->pNext;
		}
	}
}

/*******************************************************************************
Function: EnemyManagerDrawEnemies

Description: Draw the Enemy.

Inputs: ppEnemyHead - using Enemy struct.
textures[] - texture array.

Outputs: None.
*******************************************************************************/
void EnemyManagerDrawEnemies(Enemy* pEnemyHead, int textures[])
{
	WEMtx44 mtx;
	Enemy* pCur = pEnemyHead;
	char color[4];

	while (pCur)
	{
		if (pCur->state == Alive)
		{
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
			color[3] = 255;

			//As the ID change the texture
			if (pCur->ID == Sturdy)
				WEGraphicsSetTexture(textures[0]);
			else if (pCur->ID == FastWeak)
				WEGraphicsSetTexture(textures[1]);
			else if (pCur->ID == Alert)
				WEGraphicsSetTexture(textures[2]);
			else if (pCur->ID == Big)
				WEGraphicsSetTexture(textures[3]);
			else if (pCur->ID == Small)
				WEGraphicsSetTexture(textures[4]);
			//As the state, change the color.
			if (pCur->infected|| pCur->bombed)
			{
				if (!pCur->infected)
					WEGraphicsSetAttributeColor4(255, 0, 0, 255);
				else if (!pCur->bombed)
					WEGraphicsSetAttributeColor4(0, 255, 0, 255);
				else if (pCur->infected && pCur->bombed)
					WEGraphicsSetAttributeColor4(255,255,0,255);
			}
			else
				WEGraphicsSetAttributeColor4(255, 255, 255, 255);
			WEMtx44MakeTransform(&mtx,
				pCur->physical.scale.x,
				pCur->physical.scale.y,
				pCur->rot,
				pCur->physical.pos.x,
				pCur->physical.pos.y,
				0);
			WEGraphicsDraw(&mtx);

			WEGraphicsSetAttributeColor4( 255,255,255, 255);
		}
		//And Pass them.
		pCur = pCur->pNext;
	}
}

/*******************************************************************************
Function: EnemyManagerDestroyAllEnemy

Description: Delete all enemies.

Inputs: ppEnemyHead - using Enemy struct.

Outputs: None.
*******************************************************************************/
void EnemyManagerDestroyAllEnemy(Enemy* pEnemyHead)
{
	//toDelete the all Enemy.
	Enemy* toDelete;

	while (pEnemyHead)
	{
		toDelete = pEnemyHead;
		pEnemyHead = pEnemyHead->pNext;
		free(toDelete);
	}
}