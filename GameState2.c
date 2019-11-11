/*******************************************************************************
filename     GameState2.c
author       Minjeong.Kim, Yunhyuk.Choi, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This State is for Galileo state.

*******************************************************************************/
#include "GameState2.h"
#include "WarpEngine.h"
#include "DemoStates.h"

#include <stdlib.h> /*itoa*/

#include "YHObjects.h"
#include "YHDefensePlanets.h"
#include "YHOrbit.h"
#include "YHHUD.h"
#include "Sound.h" /*Sound Init Load Stop Release*/
#include "Core.h"
#include "Enemy.h"
#include "Generator.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Camera.h"
#include "Coins.h" /*Init ShutDown*/
#include "Collisions.h"
#include "ParticleSystem.h" /*ParticleSystem Init Update Draw Destroy*/
#include "WaveSystem.h"
#include "WarningSystem.h"

#define STAGE_WAVE_GALILEO 5

/*******************************  YunHyek    **********************************/
static int DTSpeed = DT_NORMAL;
static YH_DEFENSEPLANET* YHpPlanetHead = 0;
static int YHplanetTextures[S1PLANETTEXTURENUMBER];
static int YHtextures[S1TEXTURENUMBER];
static YH_MAINHUD backgroundHUD[HUDBACKGROUNDQTY];
static YH_FLICKER flickeringHUD[FLICKERQTY];
static YH_IABUTTON buttons[S1BUTTONNUMBER];
static YH_ORBIT orbits[S2CORENUMBER];
static YH_MOUSETRACER mouseTracer;
//HUD intiailize.
static YH_GAMESTATEHUDINFO gameStateOneHUDInfo = { .isPause = 0,
.isPlaying = 1,
.planetGalaxySelect = 0,
.orbitCloseup = 0,
.planetInstallation = 0,
.planetOneInstallMode = 0,
.planetTwoInstallMode = 0,
.planetThreeInstallMode = 0,
.planetFourInstallMode = 0,
.playershipManagement = 0 };
//frist core initiailize.
static const YH_COREMODIFIER coreOne = { .rot = 0.f,
.pos.x = 0.f,
.pos.y = 0.f,
.scale.x = 8.f,
.scale.y = 8.f,
.life = 21,
.orbitNumber = 7,
.orbitOffset = 5.f,
.betweenCoreFirstOrbit = 5.f,
.rotationSpeed = 0.5f };
// second core initialize
static const YH_COREMODIFIER coreTwo = { .rot = 0.f,
.pos.x = -8.f,
.pos.y = -25.f,
.scale.x = 4.f,
.scale.y = 4.f,
.life = 10,
.orbitNumber = 3,
.orbitOffset = 5.f,
.betweenCoreFirstOrbit = 5.f,
.rotationSpeed = 0.15f };
// thrid core initialize.
static const YH_COREMODIFIER coreThree = { .rot = 0.f,
.pos.x = 11.f,
.pos.y = 25.f,
.scale.x = 4.5f,
.scale.y = 4.5f,
.life = 10,
.orbitNumber = 4,
.orbitOffset = 4.f,
.betweenCoreFirstOrbit = 5.f,
.rotationSpeed = 0.15f };
// for Draw orbit.
static const YH_DRAWORBITMODIFIER drawOrbit = { .drawOrbitScale.x = 1.f,
.drawOrbitScale.y = 1.f,

/*Type 1 Draw (discarded at the moment)*/
/*.drawOrbitRadianAdd = 0.1f,
.drawOrbitGradient = 0.75f*/

/*Type 2 Draw*/
.drawOrbitDotOffset = 1.f,

/*Planet To Center Indicator*/
.drawCenterToPlanetIndicatorScale.x = 0.5f,
.drawCenterToPlanetIndicatorScale.y = 0.5f,
.drawCenterToPlanetOffset = 0.5f };

static Core cCore[S2CORENUMBER];
static Warning warningObj;
static WeaponManager weaponMag;
static Camera camera;
static MapInfo curMap;
static int YMTextures[5];
static WaveSystem* pWaveSystem;
static int coinId;
static int bulletTexture;
static int laserTexture;
static int arrowTexture;
static int siegeBulletTexture;
static float cameraZ = 30;

/*******************************************************************************
Function: GameState2Load

Description: Load the texture and sound files in the GameState2.

Inputs: pGameData - unused.

Outputs: None.
*******************************************************************************/
void GameState2Load(WEGameData* pGameData)
{
	WEUNUSED_PARAM(pGameData);

	YHTexturesLoad(YHtextures, YHplanetTextures);

	/*Sound Init and Load*/
	Sound_Init();
	Sound_Load(SOUND_SHOT, "Sounds\\shot.wav");
	Sound_Load(SOUND_STATEBGM, "Sounds\\State2BGM.wav");
	Sound_Load(SOUND_SUMMON, "Sounds\\summonPlanet.wav");
	Sound_Load(SOUND_DELETE, "Sounds\\deletePlanet.wav");
	Sound_Load(SOUND_EXPLORE, "Sounds\\enemyDeath.wav");
	Sound_Load(SOUND_LOSE, "Sounds\\Lose_Demo1.wav");
	Sound_Load(SOUND_SLUG, "Sounds\\shotSieze.wav");
	Sound_Load(SOUND_SLUGBOMB, "Sounds\\shotSiezeBomb.wav");
	Sound_Load(SOUND_POISON, "Sounds\\shotPoison.wav");
	Sound_Load(SOUND_FINALWAVE, "Sounds\\FinalWave.mp3");
	Sound_Load(SOUND_WARNNING, "Sounds\\Warn.wav");
	Sound_Load(SOUND_WIN, "Sounds\\Win.wav");
	Sound_Load(SOUND_LASER, "Sounds\\shotLaser.wav");
	Sound_Load(SOUND_ELSA, "Sounds\\shotElsa.wav");
	Sound_Load(SOUND_ENEMYAPPROACHING, "Sounds\\EnemyApproaching.mp3");
	Sound_Load(SOUND_NOTENOUGHMONEY, "Sounds\\NotEnoughMoney.mp3");
	Sound_Load(SOUND_YOUCANTDOTHAT, "Sounds\\YouCantDoThat.mp3");

	/*Particle and Bullet Texture*/
	ParticleSystem_LoadTexture(PST_circle, "Textures//particles.tga");/*earth*/
	ParticleSystem_LoadTexture(PST_smoke, "Textures//particles1.tga");/*playerass*/
	ParticleSystem_LoadTexture(PST_sparkle, "Textures//particles2.tga");/*normal*/
	ParticleSystem_LoadTexture(PST_fire, "Textures//particles3.tga");

	bulletTexture = WEGraphicsLoadTexture("Textures//bullet.tga");
	laserTexture = WEGraphicsLoadTexture("Textures//laser.tga");
	arrowTexture = WEGraphicsLoadTexture("Textures//arrowBullet.tga");
	siegeBulletTexture = WEGraphicsLoadTexture("Textures//siegeBullet.tga");

	YMTextures[0] = WEGraphicsLoadTexture("Textures//sturdy.tga");
	YMTextures[1] = WEGraphicsLoadTexture("Textures//FastWeak.tga");
	YMTextures[2] = WEGraphicsLoadTexture("Textures//FastWeak.tga");
	YMTextures[3] = WEGraphicsLoadTexture("Textures//Big.tga");
	YMTextures[4] = WEGraphicsLoadTexture("Textures//Small.tga");
	coinId = WEGraphicsLoadTexture("Textures//coin.tga");
}
/*******************************************************************************
Function: GameState2Init

Description: Initailize the all of thing we needed.

Inputs: pGameData - unused.

Outputs: None.
*******************************************************************************/
void GameState2Init(WEGameData* pGameData)
{
	//map size.
	WEVec2 numOfBlock = { 7, 7 };
	//block size.
	WEVec2 blockSize = { 20, 25 };
	int i;
	int regionNumber = 4;
	int allNumWaveAndRegion;
	int allNumEnemy;
	//Decide the num of wave.
	int a_regionsPerWave[STAGE_WAVE_GALILEO] = { 1, 1, 1, 1, 2}/* = 6 */;
	int a_placeIDs[6] = { 0 };
	//Decide the enemy property
	int a_Prop[6] = { Sturdy, FastWeak, Big, Big, Alert, Sturdy};
	//Decide the enemy pattern.
	int a_Patt[6] = { Curve, Curve, Normal, Normal, Normal, Curve};
	//Decide the Num of the enemy.
	int a_NumEnemy[6] = { 5, 10, 1, 1, 15, 10};
	allNumWaveAndRegion = 6;
	allNumEnemy = 42;
	WEGraphicsSetToPerspective();
	for (i = 0; i < allNumWaveAndRegion; ++i)
		a_placeIDs[i] = rand() % regionNumber;
	YM_InitWaveSystem(&pWaveSystem, regionNumber, STAGE_WAVE_GALILEO, 15.f, a_regionsPerWave);
	WEGraphicsSetBackGroundColor(0, 0, 0);
	/*in case the game is being restarted*/
	gameStateOneHUDInfo.isPause = 0;
	gameStateOneHUDInfo.isPlaying = 1;
	gameStateOneHUDInfo.planetGalaxySelect = 0;
	gameStateOneHUDInfo.orbitCloseup = 0;
	gameStateOneHUDInfo.planetInstallation = 0;
	gameStateOneHUDInfo.planetOneInstallMode = 0;
	gameStateOneHUDInfo.playershipManagement = 0;
	YHpPlanetHead = 0;
	//YHpEffectHead = 0;

	/*Particle System Init*/
	ParticleSystem_Init();
	Sound_BGMSetActive(1);

	/*Weapon manager Init*/
	MJWeaponManagerInit(&weaponMag);
	SetCoin(600);
	YM_InitCore(&cCore[0], &coreOne, YHtextures[REALCORE]);
	YM_InitCore(&cCore[1], &coreTwo, YHtextures[DEADCORE]);
	YM_InitCore(&cCore[2], &coreThree, YHtextures[DEADCORE]);

	/*Warning Object Init*/
	InitWarningObject(&warningObj, cCore[0].lmnt.pos, 0, 10, YHtextures[SATELLITE]);

	/*Map and Camera Init*/
	MJInitMap(&curMap, numOfBlock, blockSize);
	MJInitCamera(&camera, &curMap, cameraZ);

	/*BackGround Particle*/
	WEGraphicsSetCamera(0, 0, camera.wholeMapZ, 0);
	MJPSLoopAddEmitter(PS_BackGround, 2000, &camera.center, &camera.rot, 0, PST_sparkle,
			1.f, 3.f, -WE_MATH_PI / 9, WE_MATH_PI / 9, 1.f, 2.f, 0.15f, 0.45f);

	WEGraphicsSetCamera(0, 0, camera.initZ, 0);
	/*Wave System Init - Add region and Information*/
	YM_AddRegion(pWaveSystem, curMap.botLeft);
	YM_AddRegion(pWaveSystem, curMap.topLeft);
	YM_AddRegion(pWaveSystem, curMap.botRight);
	YM_AddRegion(pWaveSystem, curMap.topRight);
	YM_AddInfoEnemyWave(pWaveSystem, a_Prop, a_Patt, a_NumEnemy, a_placeIDs);

	/*GALAXY SELECTION BUTTONS*/
	YHInitGalaxySelectionButtons(&buttons[GALAXYSELECTIONONE], &cCore[0], &curMap, YHtextures);
	YHInitGalaxySelectionButtons(&buttons[GALAXYSELECTIONTWO], &cCore[1], &curMap, YHtextures);
	YHInitGalaxySelectionButtons(&buttons[GALAXYSELECTIONTHREE], &cCore[2], &curMap, YHtextures);

	YHInitOrthoMainHUD(backgroundHUD, YHtextures);
	YHInitIAButtons(buttons, flickeringHUD, YHtextures, S1BUTTONNUMBER, FLICKERQTY);

	/*vulerable main core*/
	InitOrbit(&orbits[0], &coreOne, &drawOrbit, YHtextures[ORBITVACANTINDICATOR], YHtextures[ORBITOCCUPIEDINDICATOR]);
	/*It's better to declare another function which initialize the invulnerable cores*/
	InitOrbit(&orbits[1], &coreTwo, &drawOrbit, YHtextures[ORBITVACANTINDICATOR], YHtextures[ORBITOCCUPIEDINDICATOR]);
	/*vulerable main core*/
	InitOrbit(&orbits[2], &coreThree, &drawOrbit, YHtextures[ORBITVACANTINDICATOR], YHtextures[ORBITOCCUPIEDINDICATOR]);
	/*MOUSE TRACER TEXTURE*/
	YHInitMouseTracer(&mouseTracer, YHReturnPlanetInfo(), YHplanetTextures);

	WEUNUSED_PARAM(pGameData);
}
/*******************************************************************************
Function: GameState2Update

Description: Update the all of thing.

Inputs: pGameData - unused.

Outputs: None.
*******************************************************************************/
void GameState2Update(WEGameData* pGameData, float dt)
{
	YH_DEFENSEPLANET* pPlanetToGet = 0;/*Planet ID distinguisher*/
	Enemy* pCurEnemy = pWaveSystem->pEnemyHead;

	WEUNUSED_PARAM(pGameData);

	/*Speed Control*/
	if (!gameStateOneHUDInfo.isPause)
	{
		if (DTSpeed == DT_NORMAL){}
		else if (DTSpeed == DT_FASTFORWARD)
			dt *= 2.f;
		else if (DTSpeed == DT_HALF)
			dt *= 0.25f;
	}
	else if (gameStateOneHUDInfo.isPause)
		dt = 0.f;

	if (WEInputIsTriggered(WE_1))
			AddCoin(800);
	if (WEInputIsTriggered(WE_2))
			MinusCoin(800);

	/*Play BGM*/
	if (Sound_BGMGetActive())
		Sound_LoopPlay(SOUND_STATEBGM);

	/*go to menu*/
	if (WEInputIsTriggered(WE_ESCAPE))
		WESMSetNextState(DS_MENU);

	/*Cheat Key*/
	if (WEInputIsTriggered(WE_F1))
		cCore[0].life = 0;
	if (WEInputIsTriggered(WE_F2))
	{
		Sound_BGMSetActive(0);
		Sound_Stop(SOUND_STATEBGM);
		curMap.CameraMode = WINEFFECT;
		Sound_Play(SOUND_WIN);
		gameStateOneHUDInfo.isPause = 1;
		gameStateOneHUDInfo.isPlaying = 0;
		gameStateOneHUDInfo.planetGalaxySelect = 0;
		gameStateOneHUDInfo.orbitCloseup = 0;
		gameStateOneHUDInfo.planetInstallation = 0;
		gameStateOneHUDInfo.planetOneInstallMode = 0;
		gameStateOneHUDInfo.planetTwoInstallMode = 0;
		gameStateOneHUDInfo.planetThreeInstallMode = 0;
		gameStateOneHUDInfo.planetFourInstallMode = 0;
		gameStateOneHUDInfo.playershipManagement = 0;

		int i = 0;
		for (i = 0; i < S1CORENUMBER; ++i)
			orbits[i].isCloseup = 0;
		mouseTracer.visible = 0;
	}
	/*lose condition, it have to be implemented in another way*/
	if (cCore[0].life <= 0)
	{
		curMap.CameraMode = LOSEEFFECT;
		gameStateOneHUDInfo.isPause = 1;
		gameStateOneHUDInfo.isPlaying = 0;
		gameStateOneHUDInfo.planetGalaxySelect = 0;
		gameStateOneHUDInfo.orbitCloseup = 0;
		gameStateOneHUDInfo.planetInstallation = 0;
		gameStateOneHUDInfo.planetOneInstallMode = 0;
		gameStateOneHUDInfo.planetTwoInstallMode = 0;
		gameStateOneHUDInfo.planetThreeInstallMode = 0;
		gameStateOneHUDInfo.planetFourInstallMode = 0;
		gameStateOneHUDInfo.playershipManagement = 0;
	}

	/*if game is not pause mode*/
	if (!gameStateOneHUDInfo.isPause)
	{
		/*restart*/
		if (WEInputIsTriggered(WE_R))
			WESMSetRestart(1);

		/* WaveSystem Update and if YM_UpdateWave == 1 -> Send Win Effect*/
		if (YM_UpdateWave(pWaveSystem, cCore[0].lmnt.pos, dt, &warningObj))
		{
			Sound_BGMSetActive(0);
			Sound_Stop(SOUND_STATEBGM);
			curMap.CameraMode = WINEFFECT;
			Sound_Play(SOUND_WIN);
			gameStateOneHUDInfo.isPause = 1;
			gameStateOneHUDInfo.isPlaying = 0;
			gameStateOneHUDInfo.planetGalaxySelect = 0;
			gameStateOneHUDInfo.orbitCloseup = 0;
			gameStateOneHUDInfo.planetInstallation = 0;
			gameStateOneHUDInfo.planetOneInstallMode = 0;
			gameStateOneHUDInfo.planetTwoInstallMode = 0;
			gameStateOneHUDInfo.planetThreeInstallMode = 0;
			gameStateOneHUDInfo.planetFourInstallMode = 0;
			gameStateOneHUDInfo.playershipManagement = 0;
		}
		//Update Enemies
		EnemyManagerUpdateEnemies(&pWaveSystem->pEnemyHead, &weaponMag, dt);

		/*Warning Update*/
		UpdateWarning(&warningObj, dt);

		/* Weapon Update*/
		MJWeaponManagerUpdate(&weaponMag, &curMap, dt);

		/*Weapon and Enemy*/
		//Set the target.
		while (pCurEnemy)
		{
			if (pCurEnemy->state == Alive)
				MJWeaponManagerSetTarget(&weaponMag, &pCurEnemy->physical.pos);
			pCurEnemy = pCurEnemy->pNext;
		}

		/*Collisions*/
		MJCREnemyPlanet(pWaveSystem->pEnemyHead, YHpPlanetHead, dt);
		MJCREnemyWeapon(pWaveSystem->pEnemyHead, &weaponMag, dt);
		MJCREnemyCore(pWaveSystem->pEnemyHead, cCore);

	}//is not pause

	/* Particle System Update*/
	ParticleSystem_Update(0.016f);
	/* Camera Update */
	MJUpdateCamera(&camera, 0.016f, S2CORENUMBER, cCore, orbits);


	/*Planet - Weapon and Particle*/
	pPlanetToGet = YHUpdateInstallDefensePlanets(&YHpPlanetHead, &gameStateOneHUDInfo, &mouseTracer, YHplanetTextures, orbits);
	/* Planet install success*/
	if (pPlanetToGet)
	{
		/*Particle emitter add*/
		MJPSLoopAddEmitter(PS_Spark, 100, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->isMoving, PST_sparkle,
			3.f, 5.f, WE_MATH_PI - WE_MATH_PI / 2.f, WE_MATH_PI + WE_MATH_PI / 2.f, 0.3f, 0.75f, 0.7f, 1.0f);

		if (pPlanetToGet->planetID == PLANET_TWO)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_GUN, bulletTexture);
		else if (pPlanetToGet->planetID == PLANET_THREE)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_LASER, laserTexture);
		else if (pPlanetToGet->planetID == PLANET_FOUR)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_FREEZ, bulletTexture);
		else if (pPlanetToGet->planetID == PLANET_FIVE)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_POISON, bulletTexture);
		else if (pPlanetToGet->planetID == PLANET_SIX)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_SIEZE, siegeBulletTexture);
		else if (pPlanetToGet->planetID == PLANET_SEVEN)
			pPlanetToGet->pWeapon = MJWeaponManagerAdd(&weaponMag, &pPlanetToGet->lmnt.pos, &pPlanetToGet->rot, &pPlanetToGet->active, EW_IRON, arrowTexture);
	}

	YHDeleteMarkedDefensePlanet(&gameStateOneHUDInfo, &weaponMag, &YHpPlanetHead, orbits, YHReturnPlanetInfo());
	YHUpdateRotateHoverSelectIndicator(dt);
	YHCheckPlanetModification(YHpPlanetHead, &gameStateOneHUDInfo, cCore, S2CORENUMBER);
	YHUpdatePlanetRotation(YHpPlanetHead, dt, YHReturnPlanetInfo());
	UpdateRotationCore(cCore, dt, S2CORENUMBER);

	/***********************************
	Update in perspective mode from here
	***********************************/
	WEGraphicsSetToPerspective();

	YHUpdatePerspectiveMouseTracer(&mouseTracer, &gameStateOneHUDInfo, orbits, S2CORENUMBER);
	YHUpdatePerspectiveCheckHoverDefPlanetsOrbitCloseup(YHpPlanetHead, &gameStateOneHUDInfo);
	YHUpdatePerspectiveCheckHoverDefPlanetsPlay(YHpPlanetHead, &gameStateOneHUDInfo);
	YHUpdatePerspectiveCheckSelectDefPlanetsPlay(YHpPlanetHead, &gameStateOneHUDInfo, cCore, S2CORENUMBER);
	YHUpdatePerspectiveMakeSelectedPlanetMovePlay(YHpPlanetHead, &gameStateOneHUDInfo, orbits);
	YHUpdateSelectedPlanetMove(YHpPlanetHead, &gameStateOneHUDInfo, orbits, dt);

	/*****************************
	Update in ortho mode from here
	*****************************/
	WEGraphicsSetToOrtho();

	YHUpdateOrthoWhichIAButtonsPressed(buttons, YHReturnPlanetInfo(), orbits, cCore, &gameStateOneHUDInfo, &curMap, &YHpPlanetHead, S1BUTTONNUMBER, S2CORENUMBER, S1GALS_CORE_OFFSET, &DTSpeed, pWaveSystem, &warningObj);
	YHUpdateCheckHoverIAButtons(buttons, S1BUTTONNUMBER);

	//draw
	GameState2Draw();
}
/*******************************************************************************
Function: GameState2Draw

Description: Draw the all of thing.

Inputs: None.

Outputs: None.
*******************************************************************************/
static void GameState2Draw(void)
{
	WEMtx44 mtx;

	/*Start drawing*/
	WEGraphicsStartDraw();
	/*************************
	Draw perspective from here
	*************************/
	WEGraphicsSetToPerspective();

	/* Particle System Draw*/
	if (curMap.CameraMode != WINEFFECT && curMap.CameraMode != LOSEEFFECT)
		ParticleSystem_Draw();

	/*Player Draw*/
	//MJDrawPlayer(&player);
	DrawWarningObj(&warningObj);
	DrawPerspectiveOrbitTypeTwo(orbits, S2CORENUMBER);

	WEGraphicsSetTexture(YHtextures[REALCORE]);
	YM_DrawCore(cCore, S2CORENUMBER);

	/*Weapon Draw*/
	MJWeaponManagerDraw(&weaponMag);

	YHDrawPerspectiveCoreToPlanetIndicator(YHpPlanetHead, orbits, &gameStateOneHUDInfo, &drawOrbit, YHtextures[CORETOPLANETINDICATOR]);
	YHDrawPerspectiveSelectPlanetsPlay(YHpPlanetHead, &gameStateOneHUDInfo, YHtextures);
	YHDrawPerspectiveDefPlanets(YHpPlanetHead);
	YHDrawPerspectiveHoverDefPlanetsCloseup(YHpPlanetHead, &gameStateOneHUDInfo, orbits, YHtextures);
	YHDrawPerspectiveMouseTracer(&gameStateOneHUDInfo, &mouseTracer);

	/* Enemy Draw*/
	EnemyManagerDrawEnemies(pWaveSystem->pEnemyHead, YMTextures);


	/*******************
	Draw ortho from here
	*******************/
	WEGraphicsSetToOrtho();

	YHDrawOrthoMainHUD(backgroundHUD, &gameStateOneHUDInfo, HUDBACKGROUNDQTY);
	YHDrawOrthoIAButtons(buttons, &gameStateOneHUDInfo, flickeringHUD, S1BUTTONNUMBER);
	YHOrthoDrawInstructionsFlickers(flickeringHUD, &gameStateOneHUDInfo);

	/*******************Here We need to rearrange code*************************************/
	int height = WEApplicationGetHeight();
	int width = WEApplicationGetWidth();


	if (pWaveSystem->waveDuration - pWaveSystem->waveTimer >= 0)
	{
		char string[100];
		_itoa_s((int)(pWaveSystem->waveDuration - pWaveSystem->waveTimer), string, 100, 10);
		WEGraphicsDrawText(string, width / 2.f, (float)height - 64.f);
	}

	WEGraphicsSetTexture(coinId);
	WEMtx44MakeTransform(&mtx, 16, 16, 0, width / 2.f - 16.f, (float)height - 32.f, 0);
	WEGraphicsDraw(&mtx);
	char string1[100];
	_itoa_s((int)GetCoin(), string1, 100, 10);
	WEGraphicsDrawText(string1, width / 2.f, (float)height - 38.f);


	WEGraphicsSetTexture(YHtextures[REALCORE]);
	WEMtx44MakeTransform(&mtx, 16, 16, 0, width / 2.f - 116.f, (float)height - 32.f, 0);
	WEGraphicsDraw(&mtx);
	char string2[100];
	_itoa_s((int)cCore[0].life, string2, 100, 10);
	WEGraphicsDrawText(string2, width / 2.f - 100.f, (float)height - 38.f);
	/**************************************************************************/

	/*End drawing*/
	WEGraphicsSetAttributeColor(0xFFFFFFFF);
	WEGraphicsSetToPerspective();
	/* Particle System Draw*/
	if (curMap.CameraMode == WINEFFECT || curMap.CameraMode == LOSEEFFECT)
		ParticleSystem_Draw();

	WEGraphicsEndDraw();
}

/*******************************************************************************
Function: GameState2Shutdown

Description: Shutdown the gameState.

Inputs: pGameData - unused.

Outputs: None.
*******************************************************************************/
void GameState2Shutdown(WEGameData* pGameData)
{
	DeleteOrbit(orbits, S2CORENUMBER);
	//YHDeleteEffectArea(YHpEffectHead);
	YHClearDefensePlanets(YHpPlanetHead);

	YHpPlanetHead = 0;

	//Enemy Destroy
	YM_DestroyWaveSystem(&pWaveSystem);

	//Reset coin
	SetCoin(0);

	//Weapon Destroy
	MJWeaponManagerDestroy(&weaponMag);

	//Particle system Destroy
	ParticleSystem_Destroy();

	//Sound reset
	Sound_Stop(SOUND_STATEBGM);
	WEUNUSED_PARAM(pGameData);
	//all sounds

}
/*******************************************************************************
Function: GameState2Unload

Description: Unload sound and texture files in the GameState.

Inputs: pGameData - unused.

Outputs: None.
*******************************************************************************/
void GameState2Unload(WEGameData* pGameData)
{
	int i;
	Sound_Release();
	ParticleSystem_Release();
	WEGraphicsUnloadTexture(bulletTexture);
	WEGraphicsUnloadTexture(laserTexture);
	WEGraphicsUnloadTexture(arrowTexture);
	WEGraphicsUnloadTexture(siegeBulletTexture);

	for (i = 0; i < 5; ++i)
		WEGraphicsUnloadTexture(YMTextures[i]);
	WEGraphicsUnloadTexture(coinId);

	for (i = 0; i < S1TEXTURENUMBER; ++i)
		WEGraphicsUnloadTexture(YHtextures[i]);
	for (i = 0; i < S1PLANETTEXTURENUMBER; ++i)
		WEGraphicsUnloadTexture(YHplanetTextures[i]);
	WEUNUSED_PARAM(pGameData);
}