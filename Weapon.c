/*******************************************************************************
filename     Weapon.c
author       Minjeong.Kim, yongmin.cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:

All Weapons Management Implement

*******************************************************************************/
#include "Weapon.h"
#include <stdlib.h> /*malloc free*/
#include "Sound.h"
#include "ParticleSystem.h"
#include "MJBullets.h"
#include "MJLaser.h"
#include "Freezing.h"
#include "Poison.h"
#include "Sieze.h"

/*******************************************************************************
Function: MakeFreezing

Description: Make the freez weapon and intialize.

Inputs: pOwner -  Substitute to freez->base.pOwner for position.
pRot - Substitute to freez->base.pRot for rotation.
pActive - Substitute to freez->base.pActive for active.

Outputs: return a pointer to freez weapon.
*******************************************************************************/
static Freezing* MakeFreezing(WEVec2* pOwner, float* pRot, int* pActive)
{
	Freezing* pNewFreezing = malloc(sizeof(Freezing));
	pNewFreezing->pNext = 0;
	pNewFreezing->base.state = Standingby;
	pNewFreezing->base.pOwner = pOwner;
	pNewFreezing->base.pRot = pRot;
	pNewFreezing->base.pActive = pActive;
	pNewFreezing->base.ID = EW_FREEZ;
	return pNewFreezing;
}

/*******************************************************************************
Function: MakeGun

Description: Make the gun weapon and intialize.

Inputs: pOwner -  Substitute to gun->base.pOwner for position.
pRot - Substitute to gun->base.pRot for rotation.
pActive - Substitute to gun->base.pActive for active.

Outputs: return a pointer to gun weapon.
*******************************************************************************/
static GUN* MakeGun(WEVec2* pOwner, float* pRot, int* pActive)
{
	GUN* pNewGun = malloc(sizeof(GUN));
	pNewGun->pNext = 0;
	pNewGun->pBulletHead = 0;
	pNewGun->base.state = Standingby;
	pNewGun->base.pOwner = pOwner;
	pNewGun->base.pRot = pRot;
	pNewGun->base.pActive = pActive;
	pNewGun->base.ID = EW_GUN;
	return pNewGun;
}
/*******************************************************************************
Function: MakeLaser

Description: Make the laser weapon and intialize.

Inputs: pOwner -  Substitute to laser->base.pOwner for position.
pRot - Substitute to laser->base.pRot for rotation.
pActive - Substitute to laser->base.pActive for active.

Outputs: return a pointer to laser weapon.
*******************************************************************************/
static LASER* MakeLaser(WEVec2* pOwner, float* pRot, int* pActive)
{
	LASER* pNewLaser = malloc(sizeof(LASER));
	pNewLaser->pNext = 0;
	pNewLaser->base.state = Standingby;
	pNewLaser->base.pOwner = pOwner;
	pNewLaser->base.pRot = pRot;
	pNewLaser->base.pActive = pActive;
	pNewLaser->base.ID = EW_LASER;
	return pNewLaser;
}

/*******************************************************************************
Function: MakePoison

Description: Make the poison planet and intialize.

Inputs: pOwner -  Substitute to pNewPoison->base.pOwner for position.
pRot - Substitute to pNewPoison->base.pRot for rotation.
pActive - Substitute to pNewPoison->base.pActive for active.

Outputs: pNewPoison - return the POISON planet.
*******************************************************************************/
static POISON* MakePoison(WEVec2* pOwner, float* pRot, int* pActive)
{
	//using malloc make pNewPoison.
	POISON* pNewPoison = malloc(sizeof(POISON));
	//Initialize the next to zero.
	pNewPoison->pNext = 0;
	//Initialize the state  to Standingby.
	pNewPoison->base.state = Standingby;
	//Initalize using parameter.
	pNewPoison->base.pOwner = pOwner;
	pNewPoison->base.pRot = pRot;
	pNewPoison->base.pActive = pActive;
	//Initilaize the enum.
	pNewPoison->base.ID = EW_POISON;
	//return the pNewPoison.
	return pNewPoison;
}

/*******************************************************************************
Function: MakeSieze

Description: Make the sieze planet and intialize.

Inputs: pOwner -  Substitute to pNewSieze->base.pOwner for position.
pRot - Substitute to pNewSieze->base.pRot for rotation.
pActive - Substitute to pNewSieze->base.pActive for active.

Outputs: pNewSieze - return the Sieze planet.
*******************************************************************************/
static Sieze* MakeSieze(WEVec2* pOwner, float* pRot, int* pActive)
{
	//using malloc make pNewSieze.
	Sieze* pNewSieze = malloc(sizeof(Sieze));
	//Initialize the next to zero.
	pNewSieze->pNext = 0;
	//Initialize the state  to Standingby.
	pNewSieze->base.state = Standingby;
	//Initalize using parameter.
	pNewSieze->base.pOwner = pOwner;
	pNewSieze->base.pRot = pRot;
	pNewSieze->base.pActive = pActive;
	//Initilaize the enum.
	pNewSieze->base.ID = EW_SIEZE;
	//return the pNewSieze.
	return pNewSieze;
}

/*******************************************************************************
Function: MakeIron

Description: Make the Iron planet and intialize.

Inputs: pOwner -  Substitute to pNewIron->base.pOwner for position.
pRot - Substitute to pNewIron->base.pRot for rotation.
pActive - Substitute to pNewIron->base.pActive for active.

Outputs: pNewIron - return the Iron planet.
*******************************************************************************/
static Iron* MakeIron(WEVec2* pOwner, float* pRot, int* pActive)
{
	//using malloc make pNewIron.
	Iron* pNewIron = malloc(sizeof(Iron));
	//Initialize the next to zero.
	pNewIron->pNext = 0;
	//Initialize the state  to Standingby.
	pNewIron->base.state = Standingby;
	//Initalize using parameter.
	pNewIron->base.pOwner = pOwner;
	pNewIron->base.pRot = pRot;
	pNewIron->base.pActive = pActive;
	//Initilaize the enum.
	pNewIron->base.ID = EW_IRON;
	//return the pNewIron.
	return pNewIron;
}

/*******************************************************************************
Function: MJWeaponManagerSetTarget

Description: Set weapons target if there are enemies in the distance.

Inputs: pWmag        -  a pointer to manager to look through the weapons that
					    weapon manager hold
		pTargetCheck - a pointer to position of target to check 
						if it is in the distance

Outputs: none.
*******************************************************************************/
void MJWeaponManagerSetTarget(WeaponManager* pWmag, WEVec2* pTargetCheck)
{
	WEVec2 direction;
	float distance;
	GUN* pCurGun = pWmag->pGunHead;
	LASER* pCurLaser = pWmag->pLaserHead;
	Freezing* pCurFreez = pWmag->pFreezingHead;
	POISON* pCurPoison = pWmag->pPoisonHead;
	Sieze* pCurSieze = pWmag->pSiezeHead;
	Iron* pCurIron = pWmag->pIronHead;

	//Each weapon is not NULL, loop this.
	while (pCurGun || pCurLaser || pCurFreez || pCurPoison || pCurSieze || pCurIron)
	{
		//if pCurGun is exist, execute this.
		if (pCurGun)
		{
		  //if satisfy this condition, execute this.
			if (pCurGun->base.available && pCurGun->base.state == Standingby)
			{
				/*calculate the distance*/
				WEVec2Sub(&direction, pTargetCheck, pCurGun->base.pOwner);
				distance = WEVec2Length(&direction);
				/*range check*/
				if (distance < pCurGun->base.range)
				{
					MJAddBullet(pCurGun, direction);
					Sound_Play(SOUND_SHOT);
					/*if pointer is not null*/
					if (pCurGun->base.pActive)
						*pCurGun->base.pActive = 1;
				}
			}
			//And pass them.
			pCurGun = pCurGun->pNext;
		}
		//if pCurLaser is exist, execute this.
		if (pCurLaser)
		{
				//if satisfy this condition, execute this.
			if (pCurLaser->base.available && pCurLaser->base.state == Standingby)
			{
				/*calculate the distance*/
				WEVec2Sub(&direction, pTargetCheck, pCurLaser->base.pOwner);
				distance = WEVec2Length(&direction);
				/*range check*/
				if (distance < pCurLaser->base.range)
				{
				  Sound_Play(SOUND_LASER);
					MJShotLaser(pCurLaser, direction);
					/*if pointer is not null*/
					if (pCurLaser->base.pActive)
						*pCurLaser->base.pActive = 1;
					return;
				}
			}
			pCurLaser = pCurLaser->pNext;
		}
		//if pCurFreez is exist, execute this.
		if (pCurFreez)
		{
				//if satisfy this condition, execute this.
			if (pCurFreez->base.available && pCurFreez->base.state == Standingby)
			{
				/*calculate the distance*/
				WEVec2Sub(&direction, pTargetCheck, pCurFreez->base.pOwner);
				distance = WEVec2Length(&direction);

				if (distance < pCurFreez->base.range)
				{
					Sound_Play(SOUND_ELSA);
					MJFreez(pCurFreez, direction);
					/*if pointer is not null*/
					if (pCurFreez->base.pActive)
						*pCurFreez->base.pActive = 1;
					return;
				}
			}
			pCurFreez = pCurFreez->pNext;
		}
		//if pCurPoison is exist, execute this.
		if (pCurPoison)
		{
				//if satisfy this condition, execute this.
			if (pCurPoison->base.available && pCurPoison->base.state == Standingby)
			{   
				/*calculate the distance*/
				WEVec2Sub(&direction, pTargetCheck, pCurPoison->base.pOwner);
				distance = WEVec2Length(&direction);
				
				/*if the target in the distance*/
				if (distance < pCurPoison->base.range)
				{
					Sound_Play(SOUND_POISON);
					/*Active Weapon*/
					YMSprayPoison(pCurPoison, direction);

					if (pCurPoison->base.pActive)
						*pCurPoison->base.pActive = 1;
					return;
				}
			}

			pCurPoison = pCurPoison->pNext;
		}
		//if pCurSieze is exist, execute this.
		if (pCurSieze)
		{
				//if satisfy this condition, execute this.
			if (pCurSieze->base.available && pCurSieze->base.state == Standingby)
			{
				/*calculate the distance*/
				WEVec2Sub(&direction, pTargetCheck, pCurSieze->base.pOwner);
				distance = WEVec2Length(&direction);

				/*if the target in the distance*/
				if (distance < pCurSieze->base.range)
				{
				  Sound_Play(SOUND_SLUG);
				  /*Active Weapon*/
					YMAddSlugBullet(pCurSieze, direction, *pTargetCheck);

					if (pCurSieze->base.pActive)
						*pCurSieze->base.pActive = 1;
					return;
				}
			}
			//And pass them.
			pCurSieze = pCurSieze->pNext;
		}
		//if pCurIron is exist, execute this.
		if (pCurIron)
		{
				//if satisfy this condition, execute this.
			if (pCurIron->base.available && pCurIron->base.state == Standingby)
			{
				WEVec2Sub(&direction, pTargetCheck, pCurIron->base.pOwner);
				distance = WEVec2Length(&direction);
				/*if the target in the distance*/
				if (distance < pCurIron->base.range)
				{/*Active Weapon*/
					YMAddIronArrow(pCurIron, direction);

					if (pCurIron->base.pActive)
						*pCurIron->base.pActive = 1;
					return;
				}
			}
			//And pass them.
			pCurIron = pCurIron->pNext;
		}
	}
}
/*******************************************************************************
Function: MJWeaponManagerInit

Description: init weapon manager by doing every head pointing null.

Inputs: pWmag   -  a pointer to manager.

Outputs: none.
*******************************************************************************/
void MJWeaponManagerInit(WeaponManager* pWmag)
{
	pWmag->pGunHead = 0;
	pWmag->pLaserHead = 0;
	pWmag->pFreezingHead = 0;
	pWmag->pPoisonHead = 0;
	pWmag->pSiezeHead = 0;
	pWmag->pIronHead = 0;
}
/*******************************************************************************
Function: MJWeaponManagerDestroy

Description: Destroy weapons, free Every Weapons

Inputs: pWmag        -    a pointer to manager.

Outputs: none.
*******************************************************************************/
void MJWeaponManagerDestroy(WeaponManager* pWmag)
{
	GUN* toDeleteGun;
	LASER* toDeleteLaser;
	Freezing* toDeleteFreez;
	POISON* toDeletePoison;
	Sieze* toDeleteSieze;
	Iron* toDeleteIron;

	/*Weapon manager Destroy Everything*/
	while (pWmag->pGunHead)
	{
		toDeleteGun = pWmag->pGunHead;
		pWmag->pGunHead = pWmag->pGunHead->pNext;
		//this weapon has bullet then destroy all bullets also
		DestroyAllBullets(&toDeleteGun->pBulletHead);
		free(toDeleteGun);
	}
	while (pWmag->pLaserHead)
	{
		toDeleteLaser = pWmag->pLaserHead;
		pWmag->pLaserHead = pWmag->pLaserHead->pNext;
		free(toDeleteLaser);
	}
	while (pWmag->pFreezingHead)
	{
		toDeleteFreez = pWmag->pFreezingHead;
		pWmag->pFreezingHead = pWmag->pFreezingHead->pNext;
		free(toDeleteFreez);
	}
	while (pWmag->pPoisonHead)
	{
		toDeletePoison = pWmag->pPoisonHead;
		pWmag->pPoisonHead = pWmag->pPoisonHead->pNext;
		free(toDeletePoison);
	}
	while (pWmag->pSiezeHead)
	{
		toDeleteSieze = pWmag->pSiezeHead;
		//this weapon has bullet then destroy all bullets also
		DestroyAllBullets(&toDeleteSieze->pSlugBulletHead);
		pWmag->pSiezeHead = pWmag->pSiezeHead->pNext;
		free(toDeleteSieze);
	}
	while (pWmag->pIronHead)
	{
		toDeleteIron = pWmag->pIronHead;
		//this weapon has bullet then destroy all bullets also
		DestroyAllBullets(&toDeleteIron->pIronArrowHead);
		pWmag->pIronHead = pWmag->pIronHead->pNext;
		free(toDeleteIron);
	}
}

//add front
/*******************************************************************************
Function: MJWeaponManagerAdd

Description: Add weapon according to ID

Inputs: pWmag        -    a pointer to manager.
		pOwner       - a pointer to weapon's owner
		pRot         - a pointer to weapon's owner's rotation
		pActive      - a pointer to owner's activation trigger
		ID           - the weapon ID
		textureID    - the weapon bullet's textureID

Outputs: a pointer to Weapon base.
*******************************************************************************/
Weapon* MJWeaponManagerAdd(WeaponManager* pWmag, WEVec2* pOwner, float* pRot, int* pActive, E_Weapon ID, int textureId)
{
	GUN* pNewGun;
	LASER* pNewLaser;
	Freezing* pNewFreez;
	POISON* pNewPoison;
	Sieze* pNewSieze;
	Iron* pNewIron;

	if (ID == EW_GUN)
	{
		pNewGun = MakeGun(pOwner, pRot, pActive);
		pNewGun->pNext = pWmag->pGunHead;
		pWmag->pGunHead = pNewGun;
		pNewGun->base.Timer = 0;
		pNewGun->base.duration = 0.15f;
		pNewGun->base.range = 20.f;
		pNewGun->base.available = 1;
		pNewGun->base.damage = 1.f;
		MJInitGun(pNewGun, textureId);
		return &pNewGun->base;
	}
	else if (ID == EW_LASER)
	{
		pNewLaser = MakeLaser(pOwner, pRot, pActive);
		pNewLaser->pNext = pWmag->pLaserHead;
		pWmag->pLaserHead = pNewLaser;
		pNewLaser->base.Timer = 0;
		pNewLaser->base.duration = 2.f;
		pNewLaser->base.range = 40.f;
		pNewLaser->base.available = 1;
		pNewLaser->base.damage = 1.f;
		MJInitLaser(pNewLaser, textureId);
		return &pNewLaser->base;
	}
	else if (ID == EW_FREEZ)
	{
		pNewFreez = MakeFreezing(pOwner, pRot, pActive);
		pNewFreez->pNext = pWmag->pFreezingHead;
		pWmag->pFreezingHead = pNewFreez;
		pNewFreez->base.damage = 1.f;
		pNewFreez->base.Timer = 0;
		pNewFreez->base.duration = 9.f;
		pNewFreez->base.range = 15.f;
		pNewFreez->base.available = 1;
		MJInitFreezing(pNewFreez);
		return &pNewFreez->base;
	}

	else if (ID == EW_POISON)
	{
		pNewPoison = MakePoison(pOwner, pRot, pActive);
		pNewPoison->pNext = pWmag->pPoisonHead;
		pWmag->pPoisonHead = pNewPoison;

		pNewPoison->base.Timer = 0;
		pNewPoison->base.duration = 9.f;
		pNewPoison->base.range = 15.f;
		pNewPoison->base.available = 1;
		YMInitPoison(pNewPoison);
		return &pNewPoison->base;
	}

	else if (ID == EW_SIEZE)
	{
		pNewSieze = MakeSieze(pOwner, pRot, pActive);
		pNewSieze->pNext = pWmag->pSiezeHead;
		pWmag->pSiezeHead = pNewSieze;

		pNewSieze->base.Timer = 0;
		pNewSieze->base.duration = 9.f;
		pNewSieze->base.range = 60.f;
		pNewSieze->base.available = 1;
		YMInitializeSieze(pNewSieze, textureId);
		return &pNewSieze->base;
	}

	else if (ID == EW_IRON)
	{
		pNewIron = MakeIron(pOwner, pRot, pActive);
		pNewIron->pNext = pWmag->pIronHead;
		pWmag->pIronHead = pNewIron;

		pNewIron->base.Timer = 0;
		pNewIron->base.duration = 12.f;
		pNewIron->base.range = 70.f;
		pNewIron->base.available = 1;
		YMInitilaizeIron(pNewIron, textureId);
		return &pNewIron->base;
	}

	return 0;
}

/*******************************************************************************
Function: MJWeaponManagerDeleteByOwner

Description: Delete the Weapon planet.

Inputs: pWmag - a pointer to manager to look through the weapons that
weapon manager hold
pOwner - for check the position.

Outputs: none.
*******************************************************************************/
void MJWeaponManagerDeleteByOwner(WeaponManager* pWmag, WEVec2* pOwner)
{
	//Gun current list.
	GUN* pCurGun;
	//Gun previous list.
	GUN* pPrevGun = 0;
	//for delete Gun list.
	GUN* toDeleteGun;
	//Laser current list.
	LASER* pCurLaser;
	//Laser previous list.
	LASER* pPrevLaser = 0;
	//Laser delete list.
	LASER* toDeleteLaser;
	//Freezing current list.
	Freezing* pCurFreezing;
	//Frezzing previous list.
	Freezing* pPrevFreezing = 0;
	//Freezing toDelete list.
	Freezing* toDeleteFreezing;
	//POISON Current list.
	POISON* pCurPoison;
	//POISON previous list.
	POISON* pPrevPoison = 0;
	//POISON toDelete list.
	POISON* toDeletePoison;
	//Sieze Current list.
	Sieze* pCurSieze;
	//Sieze previous list.
	Sieze* pPrevSieze = 0;
	//Sieze to Delete list.
	Sieze* toDeleteSieze;
	//Iron current list.
	Iron* pCurIron;
	//Iron previous list.
	Iron* pPrevIron = 0;
	//Iron to delete list.
	Iron* toDeleteIron;

	//if all of head is not exist, put off the function.
	if (!pWmag->pFreezingHead && !pWmag->pGunHead &&  !pWmag->pLaserHead && 
		!pWmag->pPoisonHead && !pWmag->pSiezeHead && !pWmag->pIronHead)
		return;

	pCurFreezing = pWmag->pFreezingHead;
	while (pCurFreezing && pCurFreezing->base.pOwner != pOwner)
	{
		pPrevFreezing = pCurFreezing;
		pCurFreezing = pCurFreezing->pNext;
	}
	if (pCurFreezing != 0)
	{	//head deleted
		if (pCurFreezing == pWmag->pFreezingHead)
		{
			toDeleteFreezing = pWmag->pFreezingHead;
			pWmag->pFreezingHead = pWmag->pFreezingHead->pNext;
			free(toDeleteFreezing);
			return;
		}
		else if (pCurFreezing != pWmag->pFreezingHead && pPrevFreezing != 0)
		{
			pPrevFreezing->pNext = pCurFreezing->pNext;
			free(pCurFreezing);
			return;
		}
	}

	pCurGun = pWmag->pGunHead;
	while (pCurGun && pCurGun->base.pOwner != pOwner)
	{
		pPrevGun = pCurGun;
		pCurGun = pCurGun->pNext;
	}

	if (pCurGun != 0)
	{	//head deleted
		if (pCurGun == pWmag->pGunHead)
		{
			toDeleteGun = pWmag->pGunHead;
			pWmag->pGunHead = pWmag->pGunHead->pNext;
			DestroyAllBullets(&toDeleteGun->pBulletHead);
			free(toDeleteGun);
			return;
		}
		else if (pCurGun != pWmag->pGunHead && pPrevGun != 0)
		{
			pPrevGun->pNext = pCurGun->pNext;
			DestroyAllBullets(&pCurGun->pBulletHead);
			free(pCurGun);
			return;
		}
	}


	pCurLaser = pWmag->pLaserHead;
	while (pCurLaser && pCurLaser->base.pOwner != pOwner)
	{
		pPrevLaser = pCurLaser;
		pCurLaser = pCurLaser->pNext;
	}

	if (pCurLaser != 0)
	{
		//head deleted
		if (!pPrevLaser)
		{
			toDeleteLaser = pWmag->pLaserHead;
			pWmag->pLaserHead = pWmag->pLaserHead->pNext;
			free(toDeleteLaser);
			return;
		}
		else if (pPrevLaser)
		{
			pPrevLaser->pNext = pCurLaser->pNext;
			free(pCurLaser);
			return;
		}
	}

	pCurPoison = pWmag->pPoisonHead;
	while (pCurPoison && pCurPoison->base.pOwner != pOwner)
	{
		pPrevPoison = pCurPoison;
		pCurPoison = pCurPoison->pNext;
	}

	if (pCurPoison != 0)
	{
		if (!pPrevPoison)
		{
			toDeletePoison = pWmag->pPoisonHead;
			pWmag->pPoisonHead = pWmag->pPoisonHead->pNext;
			free(toDeletePoison);
			return;
		}
		else if (pPrevPoison)
		{
			pPrevPoison->pNext = pCurPoison->pNext;
			free(pCurPoison);
			return;
		}
	}

	pCurSieze = pWmag->pSiezeHead;
	while (pCurSieze && pCurSieze->base.pOwner != pOwner)
	{
		pPrevSieze = pCurSieze;
		pCurSieze = pCurSieze->pNext;
	}

	if (pCurSieze != 0)
	{
		if (!pPrevSieze)
		{
			toDeleteSieze = pWmag->pSiezeHead;
			DestroyAllBullets(&toDeleteSieze->pSlugBulletHead);
			pWmag->pSiezeHead = pWmag->pSiezeHead->pNext;
			free(toDeleteSieze);
			return;
		}
		else if (pPrevSieze)
		{
			pPrevSieze->pNext = pCurSieze->pNext;
			DestroyAllBullets(&pCurSieze->pSlugBulletHead);
			free(pCurSieze);
			return;
		}
	}

	pCurIron = pWmag->pIronHead;
	while (pCurIron && pCurIron->base.pOwner != pOwner)
	{
		pPrevIron = pCurIron;
		pCurIron = pCurIron->pNext;
	}

	if (pCurIron != 0)
	{
		if (!pPrevIron)
		{
			toDeleteIron = pWmag->pIronHead;
			DestroyAllBullets(&toDeleteIron->pIronArrowHead);
			pWmag->pIronHead = pWmag->pIronHead->pNext;
			free(toDeleteIron);
			return;
		}
		else if (pPrevIron)
		{
			pPrevIron->pNext = pCurIron->pNext;
			DestroyAllBullets(&pCurIron->pIronArrowHead);
			free(pCurIron);
			return;
		}
	}
}

/*******************************************************************************
Function: MJWeaponManagerUpdate

Description: Update the weapon.

Inputs: pWmag -  a pointer to manager to look through the weapons that
weapon manager hold
pCurMap - for checking present position is in the map.

Outputs: none.
*******************************************************************************/
void MJWeaponManagerUpdate(WeaponManager* pWmag, MapInfo* pCurMap, float dt)
{
		//This four is substitute each Head of list.
	GUN* pCurGun = pWmag->pGunHead;
	LASER* pCurLaser = pWmag->pLaserHead;
	Freezing* pCurFreezing = pWmag->pFreezingHead;
	POISON* pCurPoison = pWmag->pPoisonHead;
	Sieze* pCurSieze = pWmag->pSiezeHead;
	Iron* pCurIron = pWmag->pIronHead;

	//if six exist, Update the Bullets.
	while (pCurGun)
	{
		MJUpdateGun(pCurGun, pCurMap, dt);
		pCurGun = pCurGun->pNext;
	}
	while (pCurLaser)
	{
		if (pCurLaser->base.available)
			MJUpdateLaser(pCurLaser, dt);
		pCurLaser = pCurLaser->pNext;
	}
	while (pCurFreezing)
	{
		MJUpdateFreezing(pCurFreezing, dt);
		pCurFreezing = pCurFreezing->pNext;
	}
	while (pCurPoison)
	{
		YMUpdatePoison(pCurPoison, dt);
		pCurPoison = pCurPoison->pNext;
	}
	while (pCurSieze)
	{
		YMUpdateSieze(pCurSieze, dt);
		pCurSieze = pCurSieze->pNext;
	}
	while (pCurIron)
	{
		YMUpdateIronShot(pCurIron, pCurMap,dt);
		pCurIron = pCurIron->pNext;
	}
}

/*******************************************************************************
Function: MJWeaponManagerDraw

Description: Draw the Weapon planets.

Inputs: pWmag -  a pointer to manager to look through the weapons that
weapon manager hold

Outputs: none.
*******************************************************************************/
void MJWeaponManagerDraw(WeaponManager* pWmag)
{
	//This four is substitute each Head of list.
	GUN* pCurGun = pWmag->pGunHead;
	LASER* pCurLaser = pWmag->pLaserHead;
	Sieze* pCurSieze = pWmag->pSiezeHead;
	Iron* pCurIron = pWmag->pIronHead;

	//if four exist, draw the Bullets.
	while (pCurGun)
	{
		MJDrawGun(pCurGun);
		pCurGun = pCurGun->pNext;
	}
	while (pCurLaser)
	{
		MJDrawLaser(pCurLaser);
		pCurLaser = pCurLaser->pNext;
	}
	while (pCurSieze)
	{
		YMDrawSlugBullet(pCurSieze);
		pCurSieze = pCurSieze->pNext;
	}
	while (pCurIron)
	{
		YMDrawIronArrow(pCurIron);
		pCurIron = pCurIron->pNext;
	}
}